#!/usr/bin/env python3
"""
Test generator for AtCoder Heuristic Contest - Robot Ball Cleanup.

Generates random test cases following the exact input generation specification
from the problem statement. Each test case is deterministically reproducible
via a seed parameter.

Usage:
    python test_generator.py --seed 42
    python test_generator.py --seed 0 --count 20
"""

import argparse
import random
import sys
from collections import deque


def generate_test(seed: int) -> str:
    """
    Generate a single test case from the given seed.

    Steps:
      1. Determine N = rand(10, 20).
      2. Generate walls on the grid.
      3. Determine M = round(N/2 * 4^rand_double(0,1)).
      4. Shuffle N^2 cells; first M = ball positions, next M = basket positions.
      5. Compute T using BFS shortest path distances.
      6. Format and return the input string.

    Args:
        seed: Random seed for reproducibility.

    Returns:
        The test case input as a string.
    """
    rng = random.Random(seed)

    # ----------------------------------------------------------------
    # Step 1: Grid size
    # ----------------------------------------------------------------
    N = rng.randint(10, 20)

    # ----------------------------------------------------------------
    # Step 2: Wall generation
    # ----------------------------------------------------------------
    # Walls are stored as two 2D arrays:
    #   v_walls[i][j] = True  means wall between (i,j) and (i,j+1)  (vertical wall)
    #     for 0 <= i < N, 0 <= j < N-1
    #   h_walls[i][j] = True  means wall between (i,j) and (i+1,j)  (horizontal wall)
    #     for 0 <= i < N-1, 0 <= j < N
    v_walls = [[False] * (N - 1) for _ in range(N)]
    h_walls = [[False] * N for _ in range(N - 1)]

    W = rng.randint(0, N - 1)

    def get_edges_of_vertex(vr, vc):
        """
        Return the list of interior wall edges adjacent to vertex (vr, vc).

        Vertices are grid intersection points at (0..N) x (0..N).
        Wall-edge mapping (derived from the problem's wall encoding):
          - v_walls[i][j] (wall between cell (i,j) and (i,j+1)) connects
            vertices (i, j+1) and (i+1, j+1).
          - h_walls[i][j] (wall between cell (i,j) and (i+1,j)) connects
            vertices (i+1, j) and (i+1, j+1).

        From vertex (vr, vc), the four possible adjacent edges are:
          Up   → v_walls[vr-1][vc-1]  (valid: vr≥1, 1≤vc≤N-1)
          Down → v_walls[vr][vc-1]    (valid: vr≤N-1, 1≤vc≤N-1)
          Left → h_walls[vr-1][vc-1]  (valid: 1≤vr≤N-1, vc≥1)
          Right→ h_walls[vr-1][vc]    (valid: 1≤vr≤N-1, vc≤N-1)

        Returns list of (wall_type, wall_i, wall_j, next_vr, next_vc).
        """
        edges = []
        # Up: edge from (vr,vc) to (vr-1,vc) = v_walls[vr-1][vc-1]
        if vr >= 1 and 1 <= vc <= N - 1:
            edges.append(('v', vr - 1, vc - 1, vr - 1, vc))
        # Down: edge from (vr,vc) to (vr+1,vc) = v_walls[vr][vc-1]
        if vr <= N - 1 and 1 <= vc <= N - 1:
            edges.append(('v', vr, vc - 1, vr + 1, vc))
        # Left: edge from (vr,vc) to (vr,vc-1) = h_walls[vr-1][vc-1]
        if 1 <= vr <= N - 1 and vc >= 1:
            edges.append(('h', vr - 1, vc - 1, vr, vc - 1))
        # Right: edge from (vr,vc) to (vr,vc+1) = h_walls[vr-1][vc]
        if 1 <= vr <= N - 1 and vc <= N - 1:
            edges.append(('h', vr - 1, vc, vr, vc + 1))
        return edges

    def is_wall_set(wtype, wi, wj):
        if wtype == 'v':
            return v_walls[wi][wj]
        else:
            return h_walls[wi][wj]

    def set_wall(wtype, wi, wj):
        if wtype == 'v':
            v_walls[wi][wj] = True
        else:
            h_walls[wi][wj] = True

    def vertex_adjacent_to_wall(vr, vc):
        """Check if vertex (vr,vc) is adjacent to any existing wall."""
        for wtype, wi, wj, _, _ in get_edges_of_vertex(vr, vc):
            if is_wall_set(wtype, wi, wj):
                return True
        # Also check boundary: boundary vertices are on the outer boundary
        # The outer boundary is always walls, so vertices on the boundary
        # are adjacent to boundary walls.
        if vr == 0 or vr == N or vc == 0 or vc == N:
            return True
        return False

    def is_connected_after_walls():
        """Check if all cells are still reachable via BFS."""
        visited = [[False] * N for _ in range(N)]
        visited[0][0] = True
        queue = deque([(0, 0)])
        count = 1
        while queue:
            r, c = queue.popleft()
            for dr, dc in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
                nr, nc = r + dr, c + dc
                if 0 <= nr < N and 0 <= nc < N and not visited[nr][nc]:
                    if not has_wall_between(r, c, nr, nc):
                        visited[nr][nc] = True
                        count += 1
                        queue.append((nr, nc))
        return count == N * N

    def has_wall_between(r1, c1, r2, c2):
        """Check if there's a wall between adjacent cells (r1,c1) and (r2,c2)."""
        if r1 == r2:
            # Horizontal movement: vertical wall
            return v_walls[r1][min(c1, c2)]
        else:
            # Vertical movement: horizontal wall
            return h_walls[min(r1, r2)][c1]

    # Four directions for wall extension from a vertex:
    # 0=up, 1=right, 2=down, 3=left
    # From vertex (vr, vc), extending in direction d means we place wall segments
    # along the line in that direction.

    for _ in range(W):
        # Choose a vertex not adjacent to any existing wall
        # Interior vertices: 1 <= vr <= N-1, 1 <= vc <= N-1
        # But "not adjacent to any existing wall" -- boundary vertices are always
        # adjacent to boundary walls, so we only consider interior vertices.
        candidates = []
        for vr in range(1, N):
            for vc in range(1, N):
                if not vertex_adjacent_to_wall(vr, vc):
                    candidates.append((vr, vc))

        if not candidates:
            break  # No valid vertex found, skip remaining walls

        chosen_vertex = candidates[rng.randint(0, len(candidates) - 1)]
        vr, vc = chosen_vertex

        # Choose a direction: 0=up, 1=right, 2=down, 3=left
        direction = rng.randint(0, 3)

        # Extend wall from vertex in chosen direction until it becomes
        # adjacent to another wall or the grid boundary.
        #
        # When extending in a direction, we place wall segments one by one.
        # Each segment connects two adjacent vertices along the extension line.
        #
        # Direction 0 (up): extend vertically upward from (vr, vc)
        #   Segments: v_walls[vr-1][vc-1] (between vertices (vr,vc) and (vr-1,vc)),
        #             v_walls[vr-2][vc-1] (between (vr-1,vc) and (vr-2,vc)), etc.
        # Direction 1 (right): extend horizontally rightward
        #   Segments: h_walls[vr-1][vc] (between vertices (vr,vc) and (vr,vc+1)), etc.
        # Direction 2 (down): extend vertically downward
        #   Segments: v_walls[vr][vc-1] (between (vr,vc) and (vr+1,vc)), etc.
        # Direction 3 (left): extend horizontally leftward
        #   Segments: h_walls[vr-1][vc-1] (between (vr,vc) and (vr,vc-1)), etc.

        cur_vr, cur_vc = vr, vc
        segments_to_place = []

        if direction == 0:  # Up
            while cur_vr > 0:
                # Place segment between (cur_vr, cur_vc) and (cur_vr-1, cur_vc)
                # = v_walls[cur_vr-1][cur_vc-1]
                segments_to_place.append(('v', cur_vr - 1, vc - 1))
                cur_vr -= 1
                if vertex_adjacent_to_wall(cur_vr, cur_vc):
                    break
        elif direction == 1:  # Right
            while cur_vc < N:
                # Place segment between (cur_vr, cur_vc) and (cur_vr, cur_vc+1)
                # = h_walls[cur_vr-1][cur_vc]
                segments_to_place.append(('h', vr - 1, cur_vc))
                cur_vc += 1
                if vertex_adjacent_to_wall(cur_vr, cur_vc):
                    break
        elif direction == 2:  # Down
            while cur_vr < N:
                # Place segment between (cur_vr, cur_vc) and (cur_vr+1, cur_vc)
                # = v_walls[cur_vr][cur_vc-1]
                segments_to_place.append(('v', cur_vr, vc - 1))
                cur_vr += 1
                if vertex_adjacent_to_wall(cur_vr, cur_vc):
                    break
        elif direction == 3:  # Left
            while cur_vc > 0:
                # Place segment between (cur_vr, cur_vc) and (cur_vr, cur_vc-1)
                # = h_walls[cur_vr-1][cur_vc-1]
                segments_to_place.append(('h', vr - 1, cur_vc - 1))
                cur_vc -= 1
                if vertex_adjacent_to_wall(cur_vr, cur_vc):
                    break

        # Place all segments, but verify connectivity first
        for wtype, wi, wj in segments_to_place:
            set_wall(wtype, wi, wj)

        # Verify connectivity; if broken, undo
        if not is_connected_after_walls():
            for wtype, wi, wj in segments_to_place:
                if wtype == 'v':
                    v_walls[wi][wj] = False
                else:
                    h_walls[wi][wj] = False

    # ----------------------------------------------------------------
    # Step 3: Generate M
    # ----------------------------------------------------------------
    M = round(N / 2 * (4 ** rng.uniform(0, 1)))

    # Clamp M to valid range: N/2 <= M <= 2N, and M <= N^2/2
    # (we need at least 2M distinct cells)
    M = max(round(N / 2), min(M, 2 * N, N * N // 2))

    # ----------------------------------------------------------------
    # Step 4: Ball and basket positions
    # ----------------------------------------------------------------
    all_cells = [(i, j) for i in range(N) for j in range(N)]
    rng.shuffle(all_cells)

    ball_positions = all_cells[:M]
    basket_positions = all_cells[M:2 * M]

    # ----------------------------------------------------------------
    # Step 5: Compute T
    # ----------------------------------------------------------------
    # BFS shortest path between two cells
    def bfs_distance(start, end):
        if start == end:
            return 0
        visited = {start}
        queue = deque([(start, 0)])
        while queue:
            (r, c), dist = queue.popleft()
            for dr, dc in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
                nr, nc = r + dr, c + dc
                if 0 <= nr < N and 0 <= nc < N and (nr, nc) not in visited:
                    if not has_wall_between(r, c, nr, nc):
                        if (nr, nc) == end:
                            return dist + 1
                        visited.add((nr, nc))
                        queue.append(((nr, nc), dist + 1))
        return float('inf')  # Should not happen if grid is connected

    # Sequence: (0,0) -> ball_0 -> basket_0 -> ball_1 -> basket_1 -> ...
    waypoints = [(0, 0)]
    for k in range(M):
        waypoints.append(ball_positions[k])
        waypoints.append(basket_positions[k])

    X = 0
    for i in range(len(waypoints) - 1):
        X += bfs_distance(waypoints[i], waypoints[i + 1])

    r_val = rng.uniform(0, 1)
    low_val = 2 * X + 4 * M
    high_val = 2 * N * N * M

    # T = round( low_val^r * high_val^(1-r) )
    # Handle edge case where low_val could be 0
    if low_val <= 0:
        T = round(high_val)
    else:
        import math
        T = round(math.exp(r_val * math.log(low_val) + (1 - r_val) * math.log(high_val)))

    T = max(1, T)

    # ----------------------------------------------------------------
    # Step 6: Format output
    # ----------------------------------------------------------------
    lines = []
    lines.append(f"{N} {M} {T}")

    # Vertical walls: N lines, each of length N-1
    for i in range(N):
        row_str = ''.join('1' if v_walls[i][j] else '0' for j in range(N - 1))
        lines.append(row_str)

    # Horizontal walls: N-1 lines, each of length N
    for i in range(N - 1):
        row_str = ''.join('1' if h_walls[i][j] else '0' for j in range(N))
        lines.append(row_str)

    # Ball and basket positions
    for k in range(M):
        br, bc = ball_positions[k]
        dr, dc = basket_positions[k]
        lines.append(f"{br} {bc} {dr} {dc}")

    return '\n'.join(lines) + '\n'


def main():
    parser = argparse.ArgumentParser(
        description='Generate test cases for AtCoder Heuristic Contest - Robot Ball Cleanup'
    )
    parser.add_argument(
        '--seed', type=int, default=0,
        help='Starting seed for test generation (default: 0)'
    )
    parser.add_argument(
        '--count', type=int, default=1,
        help='Number of test cases to generate (default: 1)'
    )
    parser.add_argument(
        '--outdir', type=str, default=None,
        help='Output directory for test files (default: print to stdout)'
    )
    args = parser.parse_args()

    if args.count == 1 and args.outdir is None:
        # Single test case to stdout
        print(generate_test(args.seed), end='')
    else:
        import os
        outdir = args.outdir or '.'
        os.makedirs(outdir, exist_ok=True)
        for i in range(args.count):
            seed = args.seed + i
            test_data = generate_test(seed)
            filepath = os.path.join(outdir, f'test_{i}.txt')
            with open(filepath, 'w') as f:
                f.write(test_data)
            # Print summary to stderr
            first_line = test_data.split('\n')[0]
            print(f"Seed {seed}: {first_line} -> {filepath}", file=sys.stderr)


if __name__ == '__main__':
    main()
