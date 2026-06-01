from bisect import bisect_left, bisect_right
from collections import defaultdict


class Solution:

    def calculateMaxjobProfits(self, jobStartTimes, jobEndTimes, jobProfits):
        times = []
        for i in range(len(jobStartTimes)):
            times.append((jobStartTimes[i], jobEndTimes[i], jobProfits[i]))
        times.sort(key=lambda x: (x[1], x[0]))
        dp = [0] * len(times)
        dp[0] = times[0][2]

        end_time = [x[1] for x in times]

        for i in range(1, len(times)):
            last_end_job = bisect_left(end_time, times[i][0], 0, i) - 1
            curr_profit = times[i][2]
            if last_end_job != -1:
                curr_profit += dp[last_end_job]

            dp[i] = max(dp[i-1], curr_profit)
        return dp[-1]


sol = Solution()
print(sol.calculateMaxjobProfits([1, 3, 6, 2], [3, 5, 9, 8], [20, 50, 70, 30]
                                 ))
