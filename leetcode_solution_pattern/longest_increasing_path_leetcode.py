from functools import lru_cache
from typing import List


class Solution:
    def longestIncreasingPath(self, matrix: List[List[int]]) -> int:
        dirs = [[-1, 0], [0, -1], [0, 1], [1, 0]]

        m = len(matrix)
        n = len(matrix[0])
        ans = 1

        @lru_cache(maxsize=None)
        def dfs(r: int, c: int) -> int:
            curr_path = 1
            for dr, dc in dirs:
                nr = r + dr
                nc = c + dc
                if 0 <= nr < m and 0 <= nc < n and matrix[nr][nc] > matrix[r][c]:
                    best = 1 + dfs(nr, nc)
                    curr_path = max(curr_path, best)
            return curr_path

        for i in range(m):
            for j in range(n):
                ans = max(dfs(i, j), ans)
        return ans


sol = Solution()
print(sol.longestIncreasingPath([[9, 9, 4], [6, 6, 8], [2, 1, 1]]))
