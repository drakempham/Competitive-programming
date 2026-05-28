from collections import Counter


class Solution:
    def numberOfPairs(self, nums1: list[int], nums2: list[int], queries: list[list[int]]) -> list[int]:
        S = 700
        n = len(nums2)
        nums2 = nums2[:]
        block_count = (S+n-1) // S
        cnt = [Counter() for _ in range(block_count)]
        for i, num in enumerate(nums2):
            cnt[i//S][num] += 1
        lazy_add = [0] * block_count
        ans = []
        for query in queries:
            if query[0] == 1:
                _, x, y, val = query
                while x <= y:
                    block_x = x // S
                    if x % S == 0 and (x+S) <= y:
                        lazy_add[block_x] += val
                        x += S
                    else:
                        old_val = nums2[x]
                        new_val = old_val + val

                        cnt[block_x][old_val] -= 1
                        nums2[x] = new_val
                        cnt[block_x][new_val] += 1

                        x += 1
            else:
                _, target = query
                total = 0
                for num in nums1:
                    remain = target - num
                    for i in range(block_count):
                        total += cnt[i][remain - lazy_add[i]]
                ans.append(total)
        return ans


sol = Solution()
print(sol.numberOfPairs([1, 2], [3, 4], [[2, 5], [1, 0, 0, 2], [2, 5]]))
