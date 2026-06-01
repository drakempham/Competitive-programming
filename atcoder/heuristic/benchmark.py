#!/usr/bin/env python3
"""
Benchmark runner for AtCoder Heuristic Contest - Robot Ball Cleanup.

Runs a solution against multiple generated test cases, simulates the robot
to verify correctness, computes scores, and optionally compares two solutions.

Usage:
    python benchmark.py solution.py
    python benchmark.py solution.py --count 50 --seed 0
    python benchmark.py solution.py --compare other_solution.py --count 20
"""

import argparse
import math
import os
import subprocess
import sys
import time
from collections import defaultdict

# Allow importing from the same directory
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from test_generator import generate_test


# ============================================================================
# Robot Simulator
# ============================================================================

class SimulationError(Exception):
    """Raised when the simulation encounters an invalid state."""
    pass


def parse_input(input_str: str):
    """
    Parse a test case input string into its components.

    Returns:
        dict with keys: N, M, T, v_walls, h_walls, balls, baskets
            - v_walls[i][j]: True if wall between (i,j) and (i,j+1)
            - h_walls[i][j]: True if wall between (i,j) and (i+1,j)
            - balls[k]: (row, col) of ball k
            - baskets[k]: (row, col) of basket k
    """
    lines = input_str.strip().split('\n')
    idx = 0

    parts = lines[idx].split()
    N, M, T = int(parts[0]), int(parts[1]), int(parts[2])
    idx += 1

    # Vertical walls: N lines, each length N-1
    v_walls = []
    for i in range(N):
        row = [c == '1' for c in lines[idx]]
        v_walls.append(row)
        idx += 1

    # Horizontal walls: N-1 lines, each length N
    h_walls = []
    for i in range(N - 1):
        row = [c == '1' for c in lines[idx]]
        h_walls.append(row)
        idx += 1

    # Ball and basket positions
    balls = []
    baskets = []
    for k in range(M):
        parts = lines[idx].split()
        b, c, d, e = int(parts[0]), int(parts[1]), int(parts[2]), int(parts[3])
        balls.append((b, c))
        baskets.append((d, e))
        idx += 1

    return {
        'N': N, 'M': M, 'T': T,
        'v_walls': v_walls, 'h_walls': h_walls,
        'balls': balls, 'baskets': baskets,
    }


def has_wall(v_walls, h_walls, r1, c1, r2, c2):
    """Check if there is a wall between adjacent cells (r1,c1) and (r2,c2)."""
    if r1 == r2:
        # Horizontal movement -> vertical wall
        return v_walls[r1][min(c1, c2)]
    else:
        # Vertical movement -> horizontal wall
        return h_walls[min(r1, r2)][c1]


def expand_operations(op_sequence: str) -> list:
    """
    Expand an operation sequence (with M and P) into basic operations (F, R, L, S).

    This correctly handles:
      - M: toggle macro recording
      - P: playback the most recently registered macro
      - Nested P during recording: the expanded basic ops are appended to the
        macro being recorded
      - Only basic ops F, R, L, S are in the final expanded list

    Args:
        op_sequence: String of operations (F, R, L, S, M, P)

    Returns:
        List of basic operations (each is 'F', 'R', 'L', or 'S')
    """
    registered_macro = None  # The most recently registered macro (list of basic ops)
    recording = False
    current_recording = None  # list of basic ops being recorded

    expanded = []

    for op in op_sequence:
        if op == 'M':
            if not recording:
                # Start recording
                recording = True
                current_recording = []
            else:
                # Stop recording and register the macro
                registered_macro = current_recording
                current_recording = None
                recording = False
        elif op == 'P':
            if registered_macro is None:
                # No macro registered, nothing happens
                continue
            # Play back the registered macro
            # The basic ops from the macro are executed and, if recording,
            # appended to the current recording
            for basic_op in registered_macro:
                expanded.append(basic_op)
                if recording:
                    current_recording.append(basic_op)
        elif op in ('F', 'R', 'L', 'S'):
            expanded.append(op)
            if recording:
                current_recording.append(op)
        else:
            raise SimulationError(f"Invalid operation character: '{op}'")

    return expanded


def simulate(input_str: str, output_str: str):
    """
    Simulate the robot executing the given operations on the given test case.

    Steps:
      1. Parse input to get grid, walls, ball/basket positions.
      2. Parse output to get operation sequence.
      3. Expand macros (M/P) to get basic operations.
      4. Execute at most T basic operations, simulating F/R/L/S.
      5. Count how many balls are on their corresponding baskets (V).
      6. Compute the absolute score.

    Args:
        input_str:  The test case input string.
        output_str: The solution's output string.

    Returns:
        dict with keys:
            - N, M, T: problem parameters
            - V: number of balls correctly placed
            - A: length of original operation sequence (including M and P)
            - basic_ops_executed: number of basic ops actually executed (up to T)
            - score: absolute score
            - error: error message if any, else None
    """
    data = parse_input(input_str)
    N, M, T = data['N'], data['M'], data['T']
    v_walls, h_walls = data['v_walls'], data['h_walls']

    # Parse output: each line is one operation character
    ops_raw = output_str.strip().split('\n')
    ops_raw = [line.strip() for line in ops_raw if line.strip()]
    op_sequence = ''.join(ops_raw)

    A = len(op_sequence)  # Length of operation sequence (M and P count as 1 each)

    # Validate: A must be <= T
    # Actually the problem says: "The length A of the output operation sequence must be at most T."
    # But this is about the output length, not basic ops. Let's check.
    if A > T:
        return {
            'N': N, 'M': M, 'T': T, 'V': 0, 'A': A,
            'basic_ops_executed': 0, 'score': T * M,
            'error': f'Output length {A} exceeds T={T}'
        }

    # Expand macros
    try:
        basic_ops = expand_operations(op_sequence)
    except SimulationError as e:
        return {
            'N': N, 'M': M, 'T': T, 'V': 0, 'A': A,
            'basic_ops_executed': 0, 'score': T * M,
            'error': str(e)
        }

    # Set up the grid state
    # ball_at[r][c] = ball type k, or -1 if no ball
    ball_at = [[-1] * N for _ in range(N)]
    for k in range(M):
        br, bc = data['balls'][k]
        ball_at[br][bc] = k

    # basket_at[r][c] = basket type k, or -1 if no basket
    basket_at = [[-1] * N for _ in range(N)]
    for k in range(M):
        dr, dc = data['baskets'][k]
        basket_at[dr][dc] = k

    # Robot state
    robot_r, robot_c = 0, 0
    # Direction: 0=right, 1=down, 2=left, 3=up
    robot_dir = 0
    held_ball = -1  # -1 means not holding any ball

    # Direction deltas: right, down, left, up
    DR = [0, 1, 0, -1]
    DC = [1, 0, -1, 0]

    # Execute at most T basic operations
    ops_executed = 0
    for i, op in enumerate(basic_ops):
        if ops_executed >= T:
            break

        if op == 'F':
            nr = robot_r + DR[robot_dir]
            nc = robot_c + DC[robot_dir]
            if 0 <= nr < N and 0 <= nc < N:
                if not has_wall(v_walls, h_walls, robot_r, robot_c, nr, nc):
                    robot_r, robot_c = nr, nc
            # If wall or out of bounds, robot stays in place
        elif op == 'R':
            robot_dir = (robot_dir + 1) % 4
        elif op == 'L':
            robot_dir = (robot_dir - 1) % 4
        elif op == 'S':
            cell_ball = ball_at[robot_r][robot_c]
            if held_ball == -1 and cell_ball == -1:
                # Nothing happens
                pass
            elif held_ball == -1 and cell_ball != -1:
                # Pick up ball
                held_ball = cell_ball
                ball_at[robot_r][robot_c] = -1
            elif held_ball != -1 and cell_ball == -1:
                # Put down ball
                ball_at[robot_r][robot_c] = held_ball
                held_ball = -1
            else:
                # Swap
                ball_at[robot_r][robot_c] = held_ball
                held_ball = cell_ball

        ops_executed += 1

    # Count V: balls on their corresponding baskets
    V = 0
    for k in range(M):
        dr, dc = data['baskets'][k]
        if ball_at[dr][dc] == k:
            V += 1

    # Compute score
    if V == M:
        score = A
    else:
        score = T * (M - V)

    return {
        'N': N, 'M': M, 'T': T, 'V': V, 'A': A,
        'basic_ops_executed': ops_executed,
        'score': score,
        'error': None,
    }


# ============================================================================
# Solution Runner
# ============================================================================

def run_solution(solution_path: str, input_str: str, timeout: float = 30.0):
    """
    Run a solution as a subprocess with the given input.

    Args:
        solution_path: Path to the solution file (Python or compiled binary).
        input_str: The test case input to feed via stdin.
        timeout: Maximum execution time in seconds.

    Returns:
        (output_str, elapsed_time, error_msg)
    """
    # Determine how to run the solution
    if solution_path.endswith('.py'):
        cmd = [sys.executable, solution_path]
    else:
        # Assume it's a compiled binary
        cmd = [solution_path]

    start_time = time.time()
    try:
        result = subprocess.run(
            cmd,
            input=input_str,
            capture_output=True,
            text=True,
            timeout=timeout,
        )
        elapsed = time.time() - start_time

        if result.returncode != 0:
            return None, elapsed, f"Process exited with code {result.returncode}: {result.stderr.strip()}"

        return result.stdout, elapsed, None
    except subprocess.TimeoutExpired:
        elapsed = time.time() - start_time
        return None, elapsed, f"Timeout after {timeout}s"
    except Exception as e:
        elapsed = time.time() - start_time
        return None, elapsed, str(e)


# ============================================================================
# Benchmark Runner
# ============================================================================

def run_benchmark(solution_path: str, count: int, seed_start: int,
                  timeout: float = 30.0, verbose: bool = True):
    """
    Run a solution against multiple test cases and collect results.

    Args:
        solution_path: Path to the solution file.
        count: Number of test cases.
        seed_start: Starting seed.
        timeout: Per-test timeout in seconds.
        verbose: Whether to print per-test results.

    Returns:
        List of result dicts, one per test case.
    """
    results = []

    if verbose:
        print(f"\n{'='*80}")
        print(f"  Benchmarking: {os.path.basename(solution_path)}")
        print(f"  Tests: {count} | Seeds: {seed_start}-{seed_start + count - 1}")
        print(f"{'='*80}")
        print(f"{'Seed':>6} | {'N':>3} {'M':>3} {'T':>7} | {'V':>3}/{' M':>2} | "
              f"{'Score':>10} | {'Ops':>6} | {'Time':>7} | Status")
        print(f"{'-'*80}")

    for i in range(count):
        seed = seed_start + i
        input_str = generate_test(seed)
        data = parse_input(input_str)

        output_str, elapsed, run_error = run_solution(solution_path, input_str, timeout)

        if run_error:
            result = {
                'seed': seed,
                'N': data['N'], 'M': data['M'], 'T': data['T'],
                'V': 0, 'A': 0,
                'basic_ops_executed': 0,
                'score': data['T'] * data['M'],
                'error': run_error,
                'elapsed': elapsed,
            }
        else:
            result = simulate(input_str, output_str)
            result['seed'] = seed
            result['elapsed'] = elapsed

        results.append(result)

        if verbose:
            status = '✓' if result['error'] is None else f"✗ {result['error'][:30]}"
            perfect = '★' if result['V'] == result['M'] else ' '
            print(f"{seed:>6} | {result['N']:>3} {result['M']:>3} {result['T']:>7} | "
                  f"{result['V']:>3}/{result['M']:>3}{perfect} | "
                  f"{result['score']:>10} | {result.get('basic_ops_executed', 0):>6} | "
                  f"{elapsed:>6.2f}s | {status}")

    return results


def print_summary(results: list, label: str = "Summary"):
    """Print aggregate statistics for a set of benchmark results."""
    scores = [r['score'] for r in results]
    v_vals = [r['V'] for r in results]
    m_vals = [r['M'] for r in results]
    perfect_count = sum(1 for r in results if r['V'] == r['M'])
    error_count = sum(1 for r in results if r.get('error'))
    total_balls = sum(v_vals)
    total_possible = sum(m_vals)
    elapsed_vals = [r.get('elapsed', 0) for r in results]

    print(f"\n{'='*80}")
    print(f"  {label}")
    print(f"{'='*80}")
    print(f"  Test cases:        {len(results)}")
    print(f"  Errors:            {error_count}")
    print(f"  Perfect (V=M):     {perfect_count}/{len(results)} "
          f"({100*perfect_count/len(results):.1f}%)")
    print(f"  Balls delivered:   {total_balls}/{total_possible} "
          f"({100*total_balls/total_possible:.1f}%)" if total_possible > 0 else "")
    print(f"  Score (avg):       {sum(scores)/len(scores):,.1f}")
    print(f"  Score (min):       {min(scores):,}")
    print(f"  Score (max):       {max(scores):,}")
    print(f"  Score (total):     {sum(scores):,}")
    print(f"  Score (median):    {sorted(scores)[len(scores)//2]:,}")
    print(f"  Time (avg):        {sum(elapsed_vals)/len(elapsed_vals):.2f}s")
    print(f"  Time (max):        {max(elapsed_vals):.2f}s")
    print(f"{'='*80}")


def compare_solutions(path_a: str, path_b: str, count: int, seed_start: int,
                      timeout: float = 30.0):
    """
    Run two solutions on the same test cases and compare results side-by-side.

    Args:
        path_a: Path to solution A.
        path_b: Path to solution B.
        count: Number of test cases.
        seed_start: Starting seed.
        timeout: Per-test timeout.
    """
    print(f"\n{'='*90}")
    print(f"  Comparing solutions:")
    print(f"    A: {os.path.basename(path_a)}")
    print(f"    B: {os.path.basename(path_b)}")
    print(f"  Tests: {count} | Seeds: {seed_start}-{seed_start + count - 1}")
    print(f"{'='*90}")

    results_a = []
    results_b = []

    print(f"{'Seed':>6} | {'N':>3} {'M':>3} {'T':>7} | "
          f"{'V_A':>3} {'Score_A':>10} | "
          f"{'V_B':>3} {'Score_B':>10} | {'Winner':>8}")
    print(f"{'-'*90}")

    for i in range(count):
        seed = seed_start + i
        input_str = generate_test(seed)
        data = parse_input(input_str)

        # Run solution A
        out_a, time_a, err_a = run_solution(path_a, input_str, timeout)
        if err_a:
            res_a = {
                'seed': seed, 'N': data['N'], 'M': data['M'], 'T': data['T'],
                'V': 0, 'A': 0, 'basic_ops_executed': 0,
                'score': data['T'] * data['M'], 'error': err_a, 'elapsed': time_a,
            }
        else:
            res_a = simulate(input_str, out_a)
            res_a['seed'] = seed
            res_a['elapsed'] = time_a

        # Run solution B
        out_b, time_b, err_b = run_solution(path_b, input_str, timeout)
        if err_b:
            res_b = {
                'seed': seed, 'N': data['N'], 'M': data['M'], 'T': data['T'],
                'V': 0, 'A': 0, 'basic_ops_executed': 0,
                'score': data['T'] * data['M'], 'error': err_b, 'elapsed': time_b,
            }
        else:
            res_b = simulate(input_str, out_b)
            res_b['seed'] = seed
            res_b['elapsed'] = time_b

        results_a.append(res_a)
        results_b.append(res_b)

        # Determine winner (lower score is better)
        if res_a['score'] < res_b['score']:
            winner = 'A ◀'
        elif res_b['score'] < res_a['score']:
            winner = '▶ B'
        else:
            winner = 'TIE'

        print(f"{seed:>6} | {data['N']:>3} {data['M']:>3} {data['T']:>7} | "
              f"{res_a['V']:>3} {res_a['score']:>10} | "
              f"{res_b['V']:>3} {res_b['score']:>10} | {winner:>8}")

    print_summary(results_a, f"Summary: {os.path.basename(path_a)}")
    print_summary(results_b, f"Summary: {os.path.basename(path_b)}")

    # Win/loss/tie
    a_wins = sum(1 for a, b in zip(results_a, results_b) if a['score'] < b['score'])
    b_wins = sum(1 for a, b in zip(results_a, results_b) if b['score'] < a['score'])
    ties = count - a_wins - b_wins

    print(f"\n  Head-to-head: A wins {a_wins}, B wins {b_wins}, Ties {ties}")

    # Geometric mean of score ratios (for relative comparison)
    log_ratios = []
    for a, b in zip(results_a, results_b):
        if a['score'] > 0 and b['score'] > 0:
            log_ratios.append(math.log(a['score'] / b['score']))
    if log_ratios:
        geo_mean_ratio = math.exp(sum(log_ratios) / len(log_ratios))
        if geo_mean_ratio < 1:
            print(f"  A is {1/geo_mean_ratio:.2f}x better than B on average (geometric mean)")
        else:
            print(f"  B is {geo_mean_ratio:.2f}x better than A on average (geometric mean)")


# ============================================================================
# Main
# ============================================================================

def main():
    parser = argparse.ArgumentParser(
        description='Benchmark runner for AtCoder Heuristic Contest - Robot Ball Cleanup'
    )
    parser.add_argument(
        'solution', type=str,
        help='Path to the solution file (.py or compiled binary)'
    )
    parser.add_argument(
        '--compare', type=str, default=None,
        help='Path to a second solution to compare against'
    )
    parser.add_argument(
        '--count', type=int, default=20,
        help='Number of test cases (default: 20)'
    )
    parser.add_argument(
        '--seed', type=int, default=0,
        help='Starting seed (default: 0)'
    )
    parser.add_argument(
        '--timeout', type=float, default=30.0,
        help='Per-test timeout in seconds (default: 30)'
    )
    parser.add_argument(
        '--quiet', action='store_true',
        help='Only show summary, not per-test results'
    )
    args = parser.parse_args()

    if args.compare:
        compare_solutions(
            args.solution, args.compare,
            count=args.count, seed_start=args.seed,
            timeout=args.timeout,
        )
    else:
        results = run_benchmark(
            args.solution,
            count=args.count, seed_start=args.seed,
            timeout=args.timeout, verbose=not args.quiet,
        )
        print_summary(results)


if __name__ == '__main__':
    main()
