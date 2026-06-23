

from math import sqrt


class Solution:
    def judgeSquareSum(self, c: int) -> bool:
        l = 0
        r = int(sqrt(c)) + 1

        while l <= r:
            curr = l*l + r*r
            if curr == c:
                return True
            if curr < c:
                l += 1
            else:
                r -= 1
        return False


sol = Solution()
print(sol.judgeSquareSum(5))
