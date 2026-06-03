import heapq


class Solution:
    def max_charge(charges):
        if not charges:
            return 0

        n = len(charges)
        prev = [0] * n
        next = [0] * n
        active = [True] * n
        active_count = n

        for i in range(1, n-1):
            prev[i] = i - 1
            next[i] = i + 1

        prev[0] = -1
        next[n-1] = n+1

        heap = []
        for i in range(n):
            heapq.heappush(heap, (charges[i], i))

        while len(heap) > 1:
            val, idx = heapq.heappop(heap)
