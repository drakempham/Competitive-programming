class Solution:
    def findPairs(self, nums: List[int], k: int) -> int:
        freq = Counter(nums)

        if freq == 0:
            return sum(1 for appear in freq.values() if appear >= 2)
        count = 0
        for num in freq:
            if num + k in freq:
                count += 1
        return count
