from typing import List


class Solution:
    def findThePrefixCommonArray(self, A: List[int], B: List[int]) -> List[int]:
        seen = 0
        ans = []
        count = 0
        for i in range(len(A)):
            mask_A = 1 << (A[i] - 1)
            if mask_A & seen != 0:
                count += 1
            seen = seen | mask_A

            mask_B = 1 << (B[i] - 1)
            if mask_B & seen != 0:
                count += 1
            seen = seen | mask_B
            ans.append(count)
        return ans


sol = Solution()
print(sol.findThePrefixCommonArray([1, 3, 2, 4], [3, 1, 2, 4]))


class Solution:
    def generateValidStrings(self, n: int, k: int) -> list[str]:
        ans = []

        for num in range(1 << n):
            if num & (num >> 1) != 0:
                continue

            count = 0
            temp = [0] * n
            for i in range(n):
                if (num >> i) & 1 == 1:
                    count += i
                    temp[i] = '1'
                else:
                    temp[i] = '0'

            if count > k:
                continue
            ans.append(''.join(temp))

        return ans


sol = Solution()
print(sol.generateValidStrings(3, 1))
print(sol.generateValidStrings(3, 2))
