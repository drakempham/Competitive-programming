from sortedcontainers import SortedList


class NumArray:

    def __init__(self, nums: list[int]):
        self.n = len(nums)
        self.tree = [0] * (4*self.n)
        self.build(1, 0, self.n-1, nums)
    # left right of nums, node of tree

    def build(self, node, left, right, nums):
        if left == right:
            self.tree[node] = nums[left]
            return

        mid = left + (right-left) // 2
        self.build(node*2, left, mid, nums)
        self.build(node*2+1, mid+1, right, nums)

        self.tree[node] = self.tree[node*2] + self.tree[node*2+1]

    def update(self, index: int, val: int) -> None:
        def update_tree(node: int, left: int, right: int):
            if left == right:
                self.tree[node] = val
                return

            mid = left + (right-left) // 2
            if index <= mid:
                update_tree(node*2, left, mid)
            else:
                update_tree(node*2+1, mid+1, right)

            self.tree[node] = self.tree[node*2] + self.tree[node*2+1]
        update_tree(1, 0, self.n-1)

    def sumRange(self, query_left: int, query_right: int) -> int:
        def sum_tree(node: int, left: int, right: int):
            if query_left > right or query_right < left:
                return 0
            if query_left <= left and query_right >= right:
                return self.tree[node]

            mid = left + (right-left) // 2
            sum_left = sum_tree(node*2, left, mid)
            sum_right = sum_tree(node, mid+1, right)
            return sum_left + sum_right
        return sum_tree(1, 0, self.n-1)


# Your NumArray object will be instantiated and called as such:
# obj = NumArray(nums)
# obj.update(index,val)
# param_2 = obj.sumRange(left,right)

arr = [9, -8]
seg = NumArray(arr)

print(seg.tree)
seg.update(0, 3)
print(seg.sumRange(1, 1))


class Solution:
    def __init__(self):
        self.max_x = 50000
        self.limit = self.max_x + 1

        self.seg = [0] * (4 * (self.limit + 1))
        self.st = SortedList([0, self.limit])

    def update(self, node: int, left: int, right: int, idx: int, val: int):
        if left == right:
            self.seg[node] = val
            return

        mid = left + (right - left) // 2

        if idx <= mid:
            self.update(node * 2, left, mid, idx, val)
        else:
            self.update(node * 2 + 1, mid + 1, right, idx, val)

        self.seg[node] = max(self.seg[node * 2], self.seg[node * 2 + 1])

    def query(self, node: int, left: int, right: int, query_left: int, query_right: int):
        if right < query_left or left > query_right:
            return 0

        if query_left <= left and right <= query_right:
            return self.seg[node]

        mid = left + (right - left) // 2

        max_left = self.query(node * 2, left, mid, query_left, query_right)
        max_right = self.query(node * 2 + 1, mid + 1,
                               right, query_left, query_right)

        return max(max_left, max_right)

    def getResults(self, queries: list[list[int]]) -> list[bool]:
        self.update(1, 0, self.limit, self.limit, self.limit)

        ans = []

        for query in queries:
            if query[0] == 1:
                x = query[1]

                pos = self.st.bisect_right(x)
                right = self.st[pos]
                left = self.st[pos - 1]

                self.st.add(x)

                self.update(1, 0, self.limit, x, x - left)
                self.update(1, 0, self.limit, right, right - x)

            else:
                _, x, size = query

                longest_inside = self.query(1, 0, self.limit, 0, x)

                pos = self.st.bisect_right(x)
                last_obs = self.st[pos - 1]
                longest_suffix = x - last_obs

                ans.append(max(longest_inside, longest_suffix) >= size)

        return ans


sol = Solution()
print(sol.getResults([[1, 2], [2, 3, 3], [2, 3, 1], [2, 2, 2]]))
