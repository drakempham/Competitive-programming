class ValidParentheses:
    def isValid(self, s: str) -> bool:
        mapping = {')': '(', ']': '[', '}': '{'}
        stack = []
        for ch in s:
            if ch in mapping:
                if not stack or stack.pop() != mapping[ch]:
                    return False
            else:
                stack.append(ch)
        return len(stack) == 0


sol = ValidParentheses()
print(sol.isValid("()"))


class Solution:
    def find132pattern(self, nums: list[int]) -> bool:
        stack = []
        nums_k = - 10**9

        for num in reversed(nums):
            if num < nums_k:
                return True

            while stack and num > stack[-1]:
                nums_k = max(nums_k, stack.pop())
            stack.append(num)
        return False


sol = Solution()
print(sol.find132pattern([2, 5, 1, 2, 4]))
