from typing import Counter, List


class Solution:
    def maximumLength(self, nums: List[int]) -> int:
        freq = Counter(nums)
        ans = 1

        # handle case 1
        if freq[1] >= 0:
            if freq[1] % 2 == 0:
                ans = max(ans, freq[1] - 1)
            else:
                ans = max(ans, freq[1])

        # max each loop is 5 only
        # example: 2^2 -> 2^6 < 10^9
        for num in freq.keys():
            if num == 1:
                continue

            curr_len = 0
            while freq[num] >= 2:
                curr_len += 2
                num *= num

            if freq[num] == 1:
                curr_len += 1
            else:
                curr_len -= 1

            ans = max(ans, curr_len)
        return ans


sol = Solution()
print(sol.maximumLength([1, 1, 1, 1, 1]))
print(sol.maximumLength([5, 4, 1, 2, 2]))
