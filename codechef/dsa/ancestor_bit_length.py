from collections import defaultdict, deque


cas

sol = Solution()
n = 7
v = 7
k = 2
edges = [
    [1, 2],
    [1, 4],
    [2, 5],
    [2, 3],
    [2, 6],
    [4, 7]
]
print(sol.kthAncestor(n, edges, v, k))
