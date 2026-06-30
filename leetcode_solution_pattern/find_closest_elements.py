import bisect
from typing import List


class Solution:
    def findClosestElements(self, arr: List[int], k: int, x: int) -> List[int]:
        pos = bisect.bisect_left(arr, x)
        l = pos - 1
        r = pos
        n = len(arr)

        for _ in range(k):
            if l < 0:
                r += 1
            elif r >= n:
                l -= 1
            elif x - arr[l] <= arr[r] - x:
                l -= 1
            else:
                r += 1
        return arr[l+1:r]


sol = Solution()
print(sol.findClosestElements([1, 2, 3, 4, 5], 4, 3))
