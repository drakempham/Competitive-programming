class Solution:
    def earliestFinishTime(self, landStartTime: list[int], landDuration: list[int], waterStartTime: list[int], waterDuration: list[int]) -> int:

        # assume that 1 come before 2
        def solve(start1, duration1, start2, duration2):
            finish1 = float('inf')
            for i in range(len(start1)):
                finish1 = min(finish1, start1[i] + duration1[i])

            ans = float('inf')
            for i in range(len(start2)):
                ans = min(ans, max(finish1, start2[i]) + duration2[i])
            return ans

        land_water = solve(landStartTime, landDuration,
                           waterStartTime, waterDuration)
        water_land = solve(waterStartTime, waterDuration,
                           landStartTime, landDuration)

        return min(land_water, water_land)


sol = Solution()
print(sol.earliestFinishTime([2, 8], [4, 1], [6], [3]))
