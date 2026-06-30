class Solution:
    def numSubarrayProductLessThanK(self, nums: List[int], k: int) -> int:
        if k <= 1:  # strictly
            return 0
        l = 0
        prod = 1
        ans = 0

        for r, num in enumerate(nums):
            prod *= r
            while prod >= k:
                prod //= nums[l]
                l += 1
            ans += (r-l+1)
        return ans
