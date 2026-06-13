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
    a, n = read_ints()
    digits = read_list()

    s = str(a)
    L = len(s)

    min_digit = str(digits[0])
    max_digit = str(digits[-1])

    ans = 10**30

    def relax(num_str):
        nonlocal ans

        if num_str is None:
            return

        if len(num_str) > 1 and num_str[0] == "0":
            return

        b = int(num_str)
        ans = min(ans, abs(a - b))

    def smallest_number(length):
        if length <= 0:
            return None

        if length == 1:
            return min_digit

        first_digit = None
        for d in digits:
            if d != 0:
                first_digit = str(d)
                break

        if first_digit is None:
            return None

        return first_digit + min_digit * (length - 1)

    def largest_number(length):
        if length <= 0:
            return None

        return max_digit * length

    # Different length candidates
    relax(largest_number(L - 1))
    relax(smallest_number(L + 1))

    prefix = ""

    for i, ch in enumerate(s):
        cur = int(ch)
        remaining = L - i - 1

        smaller = None
        larger = None

        for d in digits:
            if d < cur:
                smaller = d
            elif d > cur and larger is None:
                larger = d

        if smaller is not None:
            candidate = prefix + str(smaller) + max_digit * remaining
            relax(candidate)

        if larger is not None:
            candidate = prefix + str(larger) + min_digit * remaining
            relax(candidate)

        if cur in digits:
            prefix += ch
        else:
            break
    else:
        relax(s)

    print(ans)


def main():
    t = read_int()
    for _ in range(t):
        solve()


if __name__ == "__main__":
    main()
