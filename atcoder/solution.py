import sys


def solve():
    input = sys.stdin.readline

    n, m = map(int, input().split())

    cnt = [0] * (n + 1)
    eve = [[] for _ in range(m + 1)]
    for _ in range(n):
        a, d, b = map(int, input().split())
        cnt[a] += 1
        eve[d].append((a, b))

    cur_color = 0
    for c in cnt:
        if c > 0:
            cur_color += 1
    ans = []

    for day in range(1, m + 1):
        for a, b in eve[day]:
            cnt[a] -= 1
            if cnt[a] == 0:
                cur_color -= 1
            if cnt[b] == 0:
                cur_color += 1
            cnt[b] += 1

        ans.append(str(cur_color))

    print("\n".join(ans))


solve()
