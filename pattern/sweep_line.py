import heapq


class Solution:
    def getSkyline(self, buildings: list[list[int]]) -> list[list[int]]:
        edges = sorted(
            [(pos, idx) for idx, building in enumerate(buildings)
             for pos in building[:2]],
            key=lambda x: (x[0]))

        ans = []
        max_heap = []  # contains all edge actives, sorted by height
        i = 0
        while i < len(edges):
            x = edges[i][0]

            # process all edges has x
            while i < len(edges) and edges[i][0] == x:
                c_left, c_right, c_height = buildings[edges[i][1]]
                if x == c_left:  # at left of building, at to heap
                    heapq.heappush(max_heap, (-c_height, c_right))
                while max_heap and max_heap[0][1] <= x:
                    heapq.heappop(max_heap)
                i += 1

            curr_height = -max_heap[0][0] if max_heap else 0
            if not ans or ans[-1][1] != curr_height:
                ans.append((x, curr_height))
        return ans


sol = Solution()
# print(sol.getSkyline([[2, 9, 10], [3, 7, 15], [5, 12, 12], [15, 20, 10], [19, 24, 8]]
#                      ))
print(sol.getSkyline([[0, 2, 3], [2, 5, 3]]))
