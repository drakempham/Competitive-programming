# class Solution:
#     def minTimeDelivery(self, requestedHubs: list[int], transactionTimes: list[int]):
#         m, n = len(transactionTimes), len(requestedHubs)

#         prefixSum = [0] * (m+1)
#         for i in range(m):
#             prefixSum[i+1] = prefixSum[i] + transactionTimes[i]

#         def range_sum(left: int, right: int):
#             if left > right:
#                 return 0
#             return prefixSum[right+1] - prefixSum[left]

#         def cwMove(left: int, right: int):
#             if left <= right:
#                 # cong them transactionHubs tu left -> right -1
#                 return range_sum(left, right-1)

#             # cong them transaction hubs tu left -> m-1 + 0 -> right-1
#             return range_sum(left, m-1) + range_sum(0, right-1)

#         # left + 1 -> right
#         def countercwMove(left: int, right: int):
#             if left >= right:
#                 return range_sum(right+1, left)
#             return range_sum(0, left) + range_sum(right+1, m-1)

#         ans = 0
#         source = 0
#         for hub in requestedHubs:
#             next_mv = hub - 1

#             ans += min(cwMove(source, next_mv), countercwMove(source, next_mv))
#             source = next_mv
#         return ans


# sol = Solution()
# print(sol.minTimeDelivery([1, 3, 3, 2], [3, 2, 1]))
