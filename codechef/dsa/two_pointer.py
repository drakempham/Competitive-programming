class Solution:
    def subarraysWithKDistinct(self, nums: list[int], k: int) -> int:
        # since we count subarray end at position right, we can't count exactly k color
        # should change logic to
        n = len(nums)

        def countMost(colors: int):
            left, right = 0, 0
            curr_dist = 0
            ans = 0

            freq = [0] * (n+1)

            while right < n:
                if freq[nums[right]] == 0:
                    curr_dist += 1
                freq[nums[right]] += 1

                while curr_dist > colors:
                    freq[nums[left]] -= 1
                    if freq[nums[left]] == 0:
                        curr_dist -= 1
                    left += 1

                ans += (right-left + 1)
                right += 1

            return ans
        return countMost(k) - countMost(k-1)


sol = Solution()
print(sol.subarraysWithKDistinct([4, 5, 4, 5, 1], 2))
