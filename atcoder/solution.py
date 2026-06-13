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


# ---------------- Fenwick Tree ---------------- #


class Fenwick:
    def __init__(self, n):
        self.n = n
        self.bit = [0] * (n + 1)

    def add(self, idx, val):
        while idx <= self.n:
            self.bit[idx] += val
            idx += idx & -idx

    def sum(self, idx):
        res = 0
        while idx > 0:
            res += self.bit[idx]
            idx -= idx & -idx
        return res


# ---------------- Solve ---------------- #


def solve():
    N, Q = read_ints()

    r_draw = [0] * (N + 1)
    c_draw = [0] * (N + 1)
    row_bit = Fenwick(Q + 2)
    col_bit = Fenwick(Q + 2)
    row_bit.add(1, N)
    col_bit.add(1, N)

    black = 0
    out = []

    for t in range(1, Q + 1):
        typ, x = read_ints()

        if typ == 1:
            old = r_draw[x]
            count_old = col_bit.sum(old)

            black += N - count_old
            row_bit.add(old + 1, -1)
            row_bit.add(t + 1, 1)
            r_draw[x] = t

        else:
            old = c_draw[x]
            count_old = row_bit.sum(old + 1)
            count_old = N - count_old

            black -= count_old
            col_bit.add(old + 1, -1)
            col_bit.add(t + 1, 1)
            c_draw[x] = t

        out.append(str(black))

    print("\n".join(out))


def main():
    t = 1
    # t = read_int()
    for _ in range(t):
        solve()


if __name__ == "__main__":
    main()
