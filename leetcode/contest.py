from sortedcontainers import SortedList


class Solution:
    def containsNearbyAlmostDuplicate(self, nums: list[int], indexDiff: int, valueDiff: int) -> bool:
        window = SortedList()
        for i, num in enumerate(nums):
            border_left = num - valueDiff
            border_right = num + valueDiff

            idx = window.bisect_left(border_left)
            if idx != len(window) and window[idx] <= border_right:
                return True
            window.add(num)

            if i >= indexDiff:
                window.remove(nums[i-indexDiff])
        return False


sol = Solution()
# print(sol.containsNearbyAlmostDuplicate([1, 2, 3, 1], 3, 0))
print(sol.containsNearbyAlmostDuplicate([1, 2, 1, 1], 1, 0))
