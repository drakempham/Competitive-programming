import sys
from collections import defaultdict, Counter, deque
from bisect import bisect_left, bisect_right
from heapq import heapify, heappush, heappop
from math import gcd, lcm, isqrt
from functools import lru_cache

# ---------------- Debug ---------------- #


def debug(*args, sep=" ", end="\n"): print(*
                                           args, sep=sep, end=end, file=sys.stderr)

# ---------------- System ---------------- #


if hasattr(sys, "set_int_max_str_digits"):
    sys.set_int_max_str_digits(0)

sys.setrecursionlimit(300000)

input = sys.stdin.buffer.readline

# ---------------- Constants ---------------- #

MOD = 10**9 + 7

DIR4 = [(1, 0), (-1, 0), (0, 1), (0, -1)]
DIR8 = [
    (1, 0), (-1, 0), (0, 1), (0, -1),
    (1, 1), (1, -1), (-1, 1), (-1, -1)
]

# ---------------- Input helpers ---------------- #


def read_int():
    return int(input())


def read_str():
    return input().decode().strip()


def read_ints():
    return map(int, input().split())


def read_list():
    return list(map(int, input().split()))


def read_grid(n):
    return [input().decode().strip() for _ in range(n)]


def read_matrix(n):
    return [read_list() for _ in range(n)]

# ---------------- Solve ---------------- #


def solve():
    a, n = read_ints()  # n = 2 -> skip
    d1, d2 = read_ints()

    ans = float('inf')


def main():
    t = read_int()
    for _ in range(t):
        solve()


if __name__ == "__main__":
    main()
