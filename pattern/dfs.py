from collections import deque


class CheckifThereIsAValidPathInAGrid:
    def hasValidPath(self, grid: List[List[int]]) -> bool:
        path_dirs = {  # use tuple to check existence of couple
            1: [(0, -1), (0, 1)],
            2: [(-1, 0), (1, 0)],
            3: [(0, -1), (1, 0)],
            4: [(0, 1), (1, 0)],
            5: [(-1, 0), (0, -1)],
            6: [(-1, 0), (0, 1)]
        }

        visited = set()
        m, n = len(grid), len(grid[0])

        def dfs(r: int, c: int):
            if r == m-1 and c == n-1:
                return True

            visited.add((r, c))

            for dr, dc in path_dirs[grid[r][c]]:
                nr, nc = r + dr, c + dc

                if 0 <= nr < m and 0 <= nc < n:
                    if (nr, nc) in visited:
                        continue
                    if (-dr, -dc) not in path_dirs[grid[nr][nc]]:
                        continue
                    if dfs(nr, nc):
                        return True
            return False

        return dfs(0, 0)


sol = CheckifThereIsAValidPathInAGrid()
print(sol.hasValidPath([[2, 4, 3], [6, 5, 2]]))


# Definition for a binary tree node.


class TreeNode(object):
    def __init__(self, x):
        self.val = x
        self.left = None
        self.right = None


class Codec:
    # with bfs
    # def serialize(self, root):
        if not root:
            return "-"
        queue = deque([root])
        ans = []
        while queue:
            node = queue.popleft()
            if node is None:
                ans.append('-')
                continue
            ans.append(str(node.val))
            queue.append(node.left)
            queue.append(node.right)
        j = len(ans) - 1
        while j >= 0 and ans[j] == '-':
            j -= 1

    #     return ",".join(ans[:j+1])

    def deserialize(self, data):
        nodes = data.split(",")
        if nodes[0] == '-':
            return None

        root = TreeNode(nodes[0])
        i = 1
        queue = deque([root])

        while queue and i < len(nodes):
            node = queue.popleft()

            if i < len(nodes) and nodes[i] != '-':
                node.left = TreeNode(nodes[i])
                queue.append(node.left)

            i += 1

            if i < len(nodes) and nodes[i] != '-':
                node.right = TreeNode(nodes[i])
                queue.append(node.right)

            i += 1
        return root

    # with dfs
    def serialize(self, root):
        ans = []

        def dfs(node):
            if not node:
                ans.append("-")
                return

            ans.append(str(node.val))
            dfs(node.left)
            dfs(node.right)
        dfs(root)
        return ",".join(ans)

    def deserialize(self, data):
        arr = data.split(",")
        if arr[0] == "-":
            return None

        i = 0

        def dfs():
            nonlocal i

            if arr[i] == '-':
                i += 1
                return None

            node = TreeNode(int(arr[i]))
            i += 1

            node.left = dfs()
            node.right = dfs()

            return node
        return dfs()


root = TreeNode(1)
root.right = TreeNode(2)
root.right.left = TreeNode(3)
sol = Codec()
serialize_data = sol.serialize(root)
print(serialize_data)
print(sol.deserialize(serialize_data))

# print(sol.serialize(None))
# print(sol.deserialize(sol.serialize(None)))

class Solution:
    def lexicalOrder(self, n: int) -> list[int]:
        ans = []
        # dfs(i) lexographical start at i

        def dfs(num: int):
            ans.append(num)

            for j in range(10):
                new_num = num*10 + j

                if new_num > n:
                    break
                dfs(new_num)

        for i in range(1, 10):
            if i > n:
                break
            dfs(i)
        return ans


sol = Solution()
print(sol.lexicalOrder(108))
