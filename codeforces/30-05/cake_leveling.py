import sys

input = sys.stdin.readline


def solve():
    # read one test case
    n = int(input())
    arr = list(map(int, input().split()))

    # solve here
    total = 0
    ans = float('inf')
    res = []
    for i, ele in enumerate(arr, start=1):
        total += ele
        avg = total // i
        ans = min(avg, ans)
        res.append(ans)
    print(*res)


if __name__ == "__main__":
    t = int(input())
    for _ in range(t):
        solve()
