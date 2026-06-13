class Solution:
    def pivotArray(self, nums: list[int], pivot: int) -> list[int]:
        # left, right = 0, len(nums) - 1
        # i = 0

        # while i <= right:
        #     if nums[i] < pivot:
        #         nums[i], nums[left] = nums[left], nums[i]
        #         if i != left:
        #             i -= 1
        #         left += 1
        #     elif nums[i] > pivot:
        #         nums[i], nums[right] = nums[right], nums[i]
        #         i -= 1
        #         right -= 1
        #     else:
        #         pass

        #     i += 1
        # return nums[:right+1] + nums[right+1:][::-1]

        n = len(nums)
        ans = [0] * n
        left, right = 0, len(nums) - 1

        for i in range(n):
            if nums[i] < pivot:
                ans[left] = nums[i]
                left += 1

        for i in range(n-1, -1, -1):
            if nums[i] > pivot:
                ans[right] = nums[i]
                right -= 1

        while left <= right:
            ans[left] = pivot
            left += 1
        return ans


sol = Solution()
print(sol.pivotArray([9, 12, 5, 10, 14, 3, 10], 10))
