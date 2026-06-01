import sys

input = sys.stdin.readline


def solve():
    # read one test case
    n = int(input())
    arr = list(map(int, input().split()))

    # solve here
    ans = float('inf')
    for num in set(arr):
        smaller_count = 0
        larger_count = 0
        for i in range(n):
            if arr[i] < num:
                smaller_count += 1
            elif arr[i] > num:
                larger_count += 1
        ans = min(ans, max(smaller_count, larger_count))

    print("ans", ans)


if __name__ == "__main__":
    t = int(input())
    for _ in range(t):
        solve()
