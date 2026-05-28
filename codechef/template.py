from math import gcd
import sys

input = sys.stdin.readline

LOCAL = False


def debug(*args):
    if LOCAL:
        print("[DEBUG]", *args, file=sys.stderr)


def solve():
    t = int(input())

    for _ in range(1, t + 1):
        z = int(input())
        count = 0

        for x in range(2, z):
            for y in range(x + 1, z + 1):
                l = (x * y) // gcd(x, y)
                if l > z:
                    count += 1
        debug(z, count*2)
        print(count)


if __name__ == '__main__':
    solve()
