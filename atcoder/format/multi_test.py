import sys

input = sys.stdin.readline


def solve():
    # read one test case
    n = int(input())
    arr = list(map(int, input().split()))

    # solve here
    ans = 0

    print(ans)


if __name__ == "__main__":
    t = int(input())
    for _ in range(t):
        solve()
