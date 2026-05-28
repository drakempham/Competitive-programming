# Competitive Programming

This repository is my workspace for practicing competitive programming and algorithmic problem solving across platforms such as Codeforces, CodeChef, LeetCode, HackerRank, and AtCoder.

The main goal is simple: keep solved problems, reusable patterns, and contest templates in one place so I can review ideas quickly and improve over time.

## Repository Structure

```text
competitive programming/
├── atcoder/                    # AtCoder solutions and templates
├── codechef/                   # CodeChef solutions and templates
├── codeforces/                 # Codeforces practice and contest solutions
├── hackerrank/                 # HackerRank solutions
├── leetcode/                   # LeetCode solutions, mostly Java and Python
├── leetcode_solution_pattern/  # Notes for recurring LeetCode patterns
├── pattern/                    # Reusable algorithm/data structure patterns
├── LLD/                        # Low-level design practice
└── system design/              # System design notes
```

## Languages

- Python: primary language for contests and fast problem solving.
- Java: used for many LeetCode and HackerRank problems.
- SQL: used for database-style practice problems.

## How To Run

Run a Python solution with redirected input:

```bash
python3 codeforces/current.py < input.txt
```

Run a CodeChef solution:

```bash
python3 codechef/counting_lcm.py < input.txt
```

Compile and run a Java solution:

```bash
javac leetcode/04-01/MaximumSubArray.java
java -cp leetcode/04-01 MaximumSubArray
```

## Python Template

Most contest files follow this structure:

```python
import sys

input = sys.stdin.readline
LOCAL = False


def debug(*args):
    if LOCAL:
        print("[DEBUG]", *args, file=sys.stderr)


def solve():
    t = int(input())

    for _ in range(t):
        # Read input, solve, print answer
        pass


if __name__ == "__main__":
    solve()
```

`debug()` prints to `stderr`, so it is useful while testing locally without mixing debug messages with the official answer output.

## Common Patterns

The `pattern/` directory contains reusable snippets and notes for topics such as:

- Binary search
- Two pointers and sliding window
- Prefix sums
- Greedy
- Dynamic programming
- DFS, BFS, and graph traversal
- Dijkstra
- Disjoint Set Union
- Fenwick Tree
- Segment Tree
- Trie
- Stack and monotonic stack
- Bit manipulation

## Workflow

1. Put the current contest or practice solution in the platform folder.
2. Use `current.py` or `solution.py` for quick iteration.
3. Move accepted solutions into a named file after solving.
4. Add reusable ideas to `pattern/` or `leetcode_solution_pattern/`.
5. Review old solutions to recognize patterns faster in future contests.

## Notes

This is a learning repository, so some files may contain experimental code, rough drafts, or alternate attempts. The useful parts are the solved problems, reusable templates, and pattern notes that make future problem solving faster.
