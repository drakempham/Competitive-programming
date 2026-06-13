from typing import List
import collections


class Solution:
    # def consecutiveSetBits(self, n: int) -> bool:
    #     s = bin(n)[2:]
    #     count = 0

    #     for i in range(0, len(s) - 1):
    #         if s[i] == '1' and s[i+1] == '1':
    #             count += 1

    #     return count == 1
    # def consecutiveSetBits(self, n: int) -> bool:
    #     count = 0
    #     while n:
    #         if n & 3 == 3:
    #             count += 1
    #         n >>= 1
    #     return count == 1

    def consecutiveSetBits(self, n: int) -> bool:
        pairs_1 = n & (n >> 1)

        return pairs_1 > 0 and (pairs_1 & (pairs_1 - 1) == 0)


sol = Solution()
print(sol.consecutiveSetBits(6))
