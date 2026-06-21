import sys


def solve():
    data = list(map(int, sys.stdin.buffer.read().split()))
    t = data[0]
    ans = []
    p = 1

    for _ in range(t):
        m = data[p]
        n = data[p + 1]
        p += 2

        # Max number of matches that are NOT losses
        x = min(m, n)

        # Need: n - x = 2 * wins, so n - x must be even
        if (n - x) & 1:
            x -= 1

        ans.append(str(m - x))

    sys.stdout.write("\n".join(ans))


solve()
