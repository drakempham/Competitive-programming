from math import gcd
import sys

input = sys.stdin.read

LOCAL = False


def debug(*args):
    if LOCAL:
        print("[DEBUG]", *args, file=sys.stderr)


def solve():
    data = list(map(int, input().split()))
    t = data[0]
    queries = data[1:]

    max_z = max(queries)

    # smallest prime factor of z
    spf = list(range(max_z + 1))
    limit = int(max_z**0.5) + 1
    for i in range(2, limit):
        if spf[i] == i:
            for j in range(i * i, max_z + 1, i):
                if spf[j] == j:
                    spf[j] = i

    debug("spf", spf)
    # so luong uoc so cua n^2
    exact = [0] * (max_z+1)
    exact[1] = 1
    for n in range(2, max_z+1):
        p = spf[n]
        x = n
        e = 0

        while x % p == 0:
            e += 1
            x //= p
        exact[n] = exact[x] * (2*e+1)

    pref = [0] * (max_z + 1)
    for i in range(1, max_z+1):
        pref[i] = pref[i-1] + exact[i]

    for query in queries:
        print(query*query - pref[query])


if __name__ == '__main__':
    solve()


# 1
# 3
# 5
# 10
# 20
