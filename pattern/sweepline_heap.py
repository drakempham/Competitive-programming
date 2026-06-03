from collections import defaultdict


class Solution:
    def getSkyline(self, buildings: list[list[int]]) -> list[list[int]]:
        # edges = set()
        # for left, right, _ in buildings:
        #     edges.add(left)
        #     edges.add(right)

        # edges = sorted(edges)

        edges = sorted(
            set([x for building in buildings for x in building[:2]]))

        # edges_idx = defaultdict(int)
        # for i, edge in enumerate(edges):
        #     edges_idx[edge] = i

        edges_idx = {x: i for i, x in enumerate(edges)}

        heights = [0] * len(edges)

        for building in buildings:
            start = edges_idx[building[0]]
            end = edges_idx[building[1]]
            for i in range(start, end):
                heights[i] = max(heights[i], building[2])

        ans = []
        for i in range(len(heights)):
            if i == 0 or heights[i] != heights[i-1]:
                ans.append((edges[i], heights[i]))

        return ans
