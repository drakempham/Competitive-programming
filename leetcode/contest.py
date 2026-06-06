from functools import lru_cache


class Solution:
    # tc: O(nlogn)
    # sc: O(n)
    def mergeSort(self, nums, start, end) -> int:
        if start >= end:
            return 0
        mid = start + (end-start) // 2
        left_pairs = self.mergeSort(nums, start, mid)
        right_pairs = self.mergeSort(nums, mid+1, end)

        count = 0
        j = mid + 1
        for i in range(start, mid+1):
            while j <= end and nums[i] > 2 * nums[j]:
                j += 1
            count += (j - mid - 1)

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

        return left_pairs + right_pairs + count


sol = Solution()
nums = [1, 3, 2, 3, 1]
print(sol.mergeSort(nums, 0, len(nums) - 1))
print(nums)
