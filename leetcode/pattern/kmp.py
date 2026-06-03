class Solution:
    def shortestPalindrome(self, s: str) -> str:
        lps_str = s + "-" + s[::-1]

        def build_lps(text):
            lps = [0] * len(text)
            left = 0
            for right in range(1, len(text)):
                while left > 0 and text[left] != text[right]:
                    left = lps[left-1]
                if text[left] == text[right]:
                    left += 1
                lps[right] = left
            return lps

        lps = build_lps(lps_str)
        return s[lps[-1]:][::-1] + s


sol = Solution()
print(sol.shortestPalindrome("aacecaaa"))
