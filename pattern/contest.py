from collections import deque


class Solution:
    def pacificAtlantic(self, heights: list[list[int]]) -> list[list[int]]:
        ans = []
        m, n = len(heights), len(heights[0])
        pacific_visited = [[False] * n for _ in range(m)]
        atlantic_visited = [[False] * n for _ in range(m)]

        directions = [(-1, 0), (0, -1), (1, 0), (0, 1)]

        def bfs(start, visited):
            queue = deque()

            for r, c in start:
                if not visited[r][c]:
                    visited[r][c] = True
                    queue.append((r, c))

            while queue:
                curr_r, curr_c = queue.popleft()

                for dr, dc in directions:
                    nr = curr_r + dr
                    nc = curr_c + dc

                    if 0 <= nr < m and 0 <= nc < n:
                        if not visited[nr][nc] and heights[nr][nc] >= heights[curr_r][curr_c]:
                            visited[nr][nc] = True
                            queue.append((nr, nc))

        pacific_start = []
        atlantic_start = []
        for i in range(n):
            pacific_start.append([0, i])
            atlantic_start.append([m-1, i])

        for i in range(m):
            pacific_start.append([i, 0])
            atlantic_start.append([i, n-1])

        bfs(pacific_start, pacific_visited)
        bfs(atlantic_start, atlantic_visited)

        for i in range(m):
            for j in range(n):
                if pacific_visited[i][j] and atlantic_visited[i][j]:
                    ans.append([i, j])

        return ans


sol = Solution()
print(sol.pacificAtlantic([[1, 1], [1, 1], [1, 1]]))
