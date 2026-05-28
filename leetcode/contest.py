class Solution:
    def wiggleMaxLength(self, nums: list[int]) -> int:
        n = len(nums)
        if n == 1:
            return 1
        count = 1
        prev_diff, curr_diff = 0, 0
        for i in range(n-1):
            curr_diff = nums[i+1] - nums[i]

            # curr_diff decide is it allow to add or not to prevent equal num
            if (curr_diff > 0 and prev_diff <= 0) or (curr_diff < 0 and prev_diff >= 0):
                count += 1
                prev_diff = curr_diff
        return count


sol = Solution()
# print(sol.wiggleMaxLength([1, 7, 4, 9, 2, 5]))
# print(sol.wiggleMaxLength([0, 0]))
print(sol.wiggleMaxLength([1, 17, 5, 10, 13, 15, 10, 5, 16, 8]))
