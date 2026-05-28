class Solution:
    def minOperations(self, nums: list[int]) -> int:
        n = len(nums)

        # reverse and rotate left don't lost the invariant must be inc or desc
        # ex: 34012|34 reverse -> 21043|21
        # must be inc or desc type
        inc = all(
            nums[(i+1) % n] == (nums[i] + 1) % n
            for i in range(n)
        )

        desc = all(
            nums[(i+1) % n] == (nums[i] - 1 + n) % n
            for i in range(n)
        )

        if not inc and desc:
            return -1

        ans = float('inf')
        pos_0 = nums.index(0)

        # two cases to convert back to inc ( reverse or rotate first)
        # + rotate left
        # + rotate right = reverse (swap order) + rotate left + reverse(swap order)
        if inc:
            ans = min(ans, pos_0, n - pos_0 + 2)  # 2 lan rotate
        else:
            # two cases to convert back to inc ( must be reverse to swap order)
            # + rotate left and then reverse
            # + reverse then rotate left
            ans = min(ans, pos_0 + 2, 1 + n-1-pos_0)
        return ans


sol = Solution()
print(sol.minOperations([0, 2, 1]))
