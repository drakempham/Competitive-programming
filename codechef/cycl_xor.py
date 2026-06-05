from math import gcd

MOD = 998244353


def solve():
    t = int(input())

    for _ in range(t):
        n, k = map(int, input().split())

        # Real condition:
        # A[x - 2^K] xor A[x] xor A[x + 2^K] = 0
        #
        # Inside each cycle, pattern becomes:
        # a, b, a^b, a, b, a^b, ...
        #
        # Therefore cycle length must be divisible by 3.
        if n % 3 != 0:
            print(1)
            continue

        count = 0
        while n % 2 == 0:
            count += 1
            n //= 2
        g = 2**min(count, k)

        # moi cai co 4 cach chon a,b 00, 01, 10, 11
        # co tat ca k bit 0<=A[i] < 2^k
        print(pow(4, g*k, MOD))


solve()
