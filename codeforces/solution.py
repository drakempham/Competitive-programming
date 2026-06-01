import sys

input = sys.stdin.readline


def solve():
    n = int(input())
    arr = list(map(int, input().split()))

    need_layer = [0] + arr

    for i in range(1, n + 1):
        if need_layer[i] >= i:
            print("NO")
            return

    pos = [1] * (n + 1)
    mov = []

    def build(k, goal):
        if k == 0:
            return

        if pos[k] != goal[k]:
            tmp = [0] * (n + 1)

            cur = pos[k]
            nxt = goal[k]
            # no smaller layer can be at nxt.

            for layer in range(1, k):
                if layer >= k - need_layer[k]:
                    tmp[layer] = cur
                else:
                    tmp[layer] = 6 - cur - nxt

            build(k - 1, tmp)

            mov.append((k, cur, nxt))
            pos[k] = nxt

        build(k - 1, goal)

    goal = [0] * (n + 1)
    for i in range(1, n + 1):
        goal[i] = 3

    build(n, goal)

    print("YES")
    print(len(mov))
    for layer, frm, to in mov:
        print(layer, frm, to)


if __name__ == "__main__":
    t = int(input())
    for _ in range(t):
        solve()
