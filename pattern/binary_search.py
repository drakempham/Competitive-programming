import random
import bisect
from typing import List


class HIndexII:
    def hIndex(self, citations: List[int]) -> int:
        n = len(citations)
        left = 0
        right = len(citations) - 1
        while left <= right:
            mid = left + (right-left) // 2
            if citations[mid] >= n-mid:  # n-mid is h-index
                right = mid - 1
            else:
                left = mid + 1
        return n - left


sol = HIndexII()
print(sol.hIndex([0, 1, 3, 5, 6]))
print(sol.hIndex([1, 2, 100]))


class MaximizeDistanceBetweenPointsOnASquare:
    def maxDistance(self, side: int, points: List[List[int]], k: int) -> int:
        def flatten(point: List[int]):
            x, y = point[0], point[1]
            if y == 0:
                return x
            if x == side:
                return side + y
            if y == side:
                return 2*side + (side-x)
            else:  # sure on left bound
                return 3*side + (side-y)
        temp = []
        for point in points:
            temp.append((flatten(point), point[0], point[1]))
        flattern_arr = sorted(temp)
        left = 1
        right = 2*side

        # circular compare - two pointers (O(N) Optimization)
        def isSatisfyManhattan(threshold: int):
            n = len(flattern_arr)
            # Nhân đôi mảng để xử lý vòng tròn thay vì dùng %
            sorted_pts = [(p[1], p[2]) for p in flattern_arr]
            sorted_pts = sorted_pts + sorted_pts

            # Dùng Two Pointers tìm điểm tiếp theo thỏa mãn >= threshold
            next_pt = [2 * n] * (2 * n)
            j = 1
            for i in range(2 * n):
                if j <= i:
                    j = i + 1
                while j < 2 * n:
                    x = sorted_pts[i][0] - sorted_pts[j][0]
                    y = sorted_pts[i][1] - sorted_pts[j][1]
                    if abs(x) + abs(y) >= threshold:
                        break
                    j += 1
                next_pt[i] = j

            # Tìm bước nhảy index ngắn nhất
            min_diff = 2 * n + 1
            i_min = -1
            for i in range(n):
                if next_pt[i] - i < min_diff:
                    min_diff = next_pt[i] - i
                    i_min = i

            # Pigeonhole Principle: Nếu bước nhảy ngắn nhất vượt quá n // k thì không gom đủ k điểm
            if min_diff > n // k:
                return False

            # Chỉ check các điểm bắt đầu trong khoảng bước nhảy ngắn nhất
            for start_idx in range(i_min, next_pt[i_min] + 1):
                start = start_idx % n
                curr = start
                for _ in range(k):
                    curr = next_pt[curr]
                    if curr >= 2 * n:
                        break
                if curr <= start + n:
                    return True
            return False
        while left <= right:
            mid = left + (right-left) // 2
            if isSatisfyManhattan(mid):
                left = mid + 1
            else:
                right = mid - 1
        return right


sol = MaximizeDistanceBetweenPointsOnASquare()
print("MaximizeDistanceBetweenPointsOnASquare")
print(sol.maxDistance(side=2, points=[[0, 2], [2, 0], [2, 2], [0, 0]], k=4))
print(sol.maxDistance(side=2, points=[
      [0, 0], [1, 2], [2, 0], [2, 2], [2, 1]], k=4))
print(sol.maxDistance(side=4, points=[
      [4, 4], [3, 4], [2, 0], [4, 3], [4, 0]], k=4))
print(sol.maxDistance(side=15, points=[
      [0, 11], [15, 15], [0, 0], [0, 8], [14, 0]], k=4))


class Solution:
    def countKthRoots(self, l: int, r: int, k: int) -> int:
        def bound_left():  # ceil(roots_k(l))
            left, right = 0, int(l**(1/k)) + 1 if l > 0 else 0
            while left < right:
                mid = left + (right-left) // 2
                perf = mid**k
                if perf == l:
                    return mid
                if perf < l:
                    left = mid + 1
                else:
                    right = mid
            return left

        def bound_right():  # floor(roos_k(r))
            left, right = 0, int(r**(1/k)) + 1 if r > 0 else 0
            while left <= right:
                mid = left + (right-left) // 2
                perf = mid**k
                if perf == r:
                    return mid
                if perf > r:
                    right = mid - 1
                else:
                    left = mid + 1
            return left - 1
        return bound_right() - bound_left() + 1


sol = Solution()
print(sol.countKthRoots(1, 9, 3))
print(sol.countKthRoots(8, 30, 2))
print(sol.countKthRoots(19, 22, 1))
print(sol.countKthRoots(2, 3, 2))


class Solution:
    def findNthDigit(self, n: int) -> int:
        def count_digits(x: int) -> int:
            total = 0
            length = 1
            start = 1

            while start <= x:
                end = min(x, start * 10 - 1)
                total += (end - start + 1) * length

                length += 1
                start *= 10

            return total

        # Binary search smallest number whose prefix has at least n digits
        left, right = 1, n

        while left < right:
            mid = (left + right) // 2

            if count_digits(mid) >= n:
                right = mid
            else:
                left = mid + 1

        num = left

        before = count_digits(num - 1)

        idx = n - before - 1

        return int(str(num)[idx])


sol = Solution()
print(sol.findNthDigit(11))


class Solution:

    def __init__(self, rects: list[list[int]]):
        self.rects = rects

        self.pts = []

        for rect in rects:
            a, b, x, y = rect
            pts_size = (x-a+1)*(y-b+1)
            if self.pts:
                pts_size += self.pts[-1]
            self.pts.append(pts_size)
        self.total = self.pts[-1]

    def pick(self) -> list[int]:
        k = random.randint(1, self.total)
        idx = bisect.bisect_left(self.pts, k)
        a, b, x, y = self.rects[idx]
        return [random.randint(a, x), random.randint(b, y)]


sol = Solution([[-2, -2, 1, 1], [2, 2, 4, 6]])
print(sol.pick())
