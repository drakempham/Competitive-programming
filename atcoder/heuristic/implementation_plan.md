# AtCoder Heuristic Contest Macro Optimizer Plan

This document outlines the detailed strategy and implementation plan to build a solver for the AtCoder Heuristic Contest (AHC) involving ball routing and macro controller optimization.

The problem requires us to control a character/system using basic operations (`F`, `R`, `L`, `S`) and macro operations (`M` to record/stop and `P` to play) to deliver $M$ balls to their corresponding baskets.

Our primary goal is to maximize successfully placed balls ($V = M$) to avoid massive penalty scores, and our secondary goal is to minimize the controller operations count ($A$) using optimized macros.

---

## User Review Required

> [!IMPORTANT]
> Since we do not have the full grid geometry and rules (e.g., whether multiple balls are moved simultaneously, if there are static/dynamic obstacles, or the exact board dimension $N$), we will write a **modular and parameterizable pathfinder**. 
> 
> We will design the core solver in **Python** for easy iteration, plotting, and testing, and then wrap it as a clean solution.

---

## Proposed Changes

We will create a new subdirectory at `/Users/minhpd/.gemini/antigravity/scratch/ahc_macro_solver` to store our solver.

### Component 1: Routing & Pathfinding (Phase 1)
This component generates the raw, uncompressed sequence of basic movements (`F`, `R`, `L`, `S`) required to complete the puzzle.

#### [NEW] [pathfinder.py](file:///Users/minhpd/.gemini/antigravity/scratch/ahc_macro_solver/pathfinder.py)
- Implements a grid representation with obstacles.
- Uses **Breadth-First Search (BFS)** or **A\* Search** to find the shortest collision-free paths from each ball to its destination basket.
- Features a greedy scheduler that orders ball deliveries to prevent congestion or deadlocks if multiple balls are moved sequentially.
- Outputs a raw movement string $S$ consisting purely of `F`, `R`, `L`, and `S`.

---

### Component 2: Macro Compressor (Phase 2)
This component compresses the raw movement string $S$ into a highly optimized sequence of controller presses containing `M` and `P`.

#### [NEW] [compressor.py](file:///Users/minhpd/.gemini/antigravity/scratch/ahc_macro_solver/compressor.py)
To find the absolute shortest controller string, we will implement a **Beam Search** over the macro state space.

- **State Representation:**
  - `index`: Current position in the raw string $S$.
  - `active_macro`: The string of basic operations representing the currently registered macro.
  - `operations`: The sequence of controller commands (`F`, `R`, `L`, `S`, `M`, `P`) generated so far.
  - `score` (cost): The length of `operations` (which represents the current $A$ value).

- **Search Transitions:**
  - **Transition A (Append Raw):** Output the next basic operation $S[index]$ directly.
    - *Cost:* $+1$ button press.
    - *Progress:* Advances `index` by 1.
  - **Transition B (Play Macro):** If the substring of $S$ starting at `index` matches `active_macro`, output `P`.
    - *Cost:* $+1$ button press.
    - *Progress:* Advances `index` by $|active\_macro|$.
  - **Transition C (Register New Macro):** Choose a candidate substring from the remaining part of $S$, record it as a new macro using `M <content> M`.
    - To support **recursive macro lồng ghép**, the `<content>` can contain `P` to play the *previous* macro.
    - *Cost:* $+2$ button presses (for the two `M`s) plus the cost of the content.
    - *Progress:* Advances `index` by the expanded length of the new macro.
    - *Updates:* Sets `active_macro` to the expanded new macro.

- **Pruning & Beam Width:**
  - To prevent state explosion, we keep only the top $B$ (beam width, e.g., 500) states with the lowest button count $A$ for each step along the string $S$.

---

### Component 3: Local Validator & Simulator
This component allows us to verify correctness, test edge cases, and run benchmark simulations.

#### [NEW] [simulator.py](file:///Users/minhpd/.gemini/antigravity/scratch/ahc_macro_solver/simulator.py)
- Simulates the execution of the controller sequence (expanding all macros step-by-step).
- Validates that:
  1. The total expanded operations do not exceed the limit $T$.
  2. The character successfully places all balls into their corresponding baskets.
  3. No illegal moves (e.g., walking into walls) are executed.
- Computes and prints the final absolute score $A$ and validates correctness.

---

## Verification Plan

### Automated Tests
We will write a test suite in Python that generates random grid puzzles with different $N$, $M$, and obstacle densities, runs them through the solver, and asserts:
1. Complete delivery ($V = M$).
2. Validity of macro expansions.
3. Length reduction ratio (compares $A$ against the raw string length to measure compression efficiency).

To execute the tests, we will run:
```bash
python3 -m unittest discover -s /Users/minhpd/.gemini/antigravity/scratch/ahc_macro_solver -p "*_test.py"
```

### Manual Verification
- We will print the step-by-step macro expansion for a small test case to visually inspect that the compressor correctly uses nesting (e.g., `M F P M P` structure) and verify its logical soundness.
