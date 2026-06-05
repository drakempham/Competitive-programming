from functools import lru_cache


class Solution:
    def mergeSort(self, nums, start, end):
        if start >= end:
            return
        mid = start + (end-start) // 2
        self.mergeSort(nums, start, mid)
        self.mergeSort(nums, mid+1, end)

        i = start
        j = mid + 1
        arr = []
        while i <= mid and j <= end:
            if nums[i] <= nums[j]:
                arr.append(nums[i])
                i += 1
            else:
                arr.append(nums[j])
                j += 1

        while i <= mid:
            arr.append(nums[i])
            i += 1

        while j <= end:
            arr.append(nums[j])
            j += 1
        for i in range(len(arr)):
            nums[i+start] = arr[i]


sol = Solution()
nums = [1, 3, 2, 6]
sol.mergeSort(nums, 0, 3)
print(nums)
