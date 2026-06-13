from collections import deque


class Solution:
    def findMinPlatforms(self, AT: list[int], DT: list[int], n: int) -> int:
        AT.sort()
        DT.sort()

        i, j = 0, 0
        platform = 0
        ans = 0

        while i < n and j < n:
            if AT[i] < DT[j]:
                platform += 1
                i += 1
            elif AT[i] > DT[j]:
                platform -= 1
                j += 1
            else:
                i += 1
                j += 1
                platform += 1
            ans = max(ans, platform)
        return ans


sol = Solution()
print(sol.findMinPlatforms([900, 940, 950, 1100, 1500, 1800], [
      910, 1200, 1120, 1130, 1900, 2000], 6))
