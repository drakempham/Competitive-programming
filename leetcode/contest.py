class Solution:
    def processStr(self, s: str) -> str:
        res = []
        for c in s:
            if 'a' <= c <= 'z':
                res.append(c)
            elif c == '*':
                if res:
                    res.pop()
            elif c == '#':
                res.extend(res[:])
            else:  # %
                res.reverse()
        return ''.join(res)


sol = Solution()
print(sol.processStr("a#b%*"))
