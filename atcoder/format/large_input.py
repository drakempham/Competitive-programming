import sys


def solve():
    data = list(map(int, sys.stdin.buffer.read().split()))

    # example:
    # first number is t
    t = data[0]
    idx = 1

    out = []

    for _ in range(t):
        n = data[idx]
        m = data[idx + 1]
        idx += 2

        ans = 0
        while m != 0:
            m = n % m
            ans += 1

        out.append(str(ans))

    print("\n".join(out))


if __name__ == "__main__":
    solve()
