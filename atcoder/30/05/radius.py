import sys

input = sys.stdin.readline


def solve():
    # read one test case
    x1, y1, r1, x2, y2, r2 = map(int, input().split())

    distance = (y1-y2)**2 + (x1-x2)**2

    low = abs(r1-r2)
    high = r1+r2

    if low**2 <= distance <= high**2:
        print("Yes")
    else:
        print("No")


if __name__ == "__main__":
    t = int(input())
    for _ in range(t):
        solve()
