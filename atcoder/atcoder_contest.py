from collections import deque
import sys

input = sys.stdin.readline


def solve():
    h, w = map(int, input().split())
    grid = [list(input()) for _ in range(h)]

    dirs = [
        (-1, -1), (-1, 0), (-1, 1),
        (0, -1), (0, 1),
        (1, -1), (1, 0), (1, 1)
    ]

    dist = [[-1] * w for _ in range(h)]
    queue = deque()

    for r in range(h):
        for c in range(w):
            if grid[r][c] == '#':
                dist[r][c] = 0
                queue.append((r, c))

    while queue:
        r, c = queue.popleft()

        for dr, dc in dirs:
            nr, nc = r + dr, c + dc
            if 0 <= nr < h and 0 <= nc < w:
                if dist[nr][nc] == -1:
                    dist[nr][nc] = dist[r][c] + 1
                    queue.append((nr, nc))

    ans = []

    for r in range(h):
        rows = []
        for c in range(w):
            if dist[r][c] == -1:
                rows.append('.')
            elif dist[r][c] % 2 == 0:
                rows.append('#')
            else:
                rows.append('.')
        ans.append("".join(rows))

    print("\n".join(ans))


if __name__ == "__main__":
    solve()
