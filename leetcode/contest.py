class Solution:
    def countDigitOne(self, n: int) -> int:
        i = 1
        total_1 = 0
        while i <= n:
            curr_pos = (n // i) % 10
            before_pos = n // (i*10)
            after_pos = n % i

            # add cycle
            total_1 += before_pos * i

            if curr_pos == 1:  # add the root of cycle like 1134
                total_1 += after_pos + 1
            elif curr_pos > 1:
                total_1 += i

            i *= 10
        return total_1


sol = Solution()
print(sol.countDigitOne(13))
