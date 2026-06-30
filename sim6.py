def get_poly(s):
    c1, c0 = 0, 1
    for _ in range(s):
        c1, c0 = c0 - c1, -2*c1
    return c1, c0

# Find all lonely subsets of {0, 1, ..., 13}
def get_lonely(k):
    res = []
    def dfs(i, current):
        if i >= k:
            res.append(current)
            return
        # Option 1: don't include i
        dfs(i+1, current)
        # Option 2: include i
        if not current or i - current[-1] >= 3:
            dfs(i+1, current + [i])
            
    dfs(0, [])
    return res

lonely_sets = get_lonely(14)
sad_integers = set()

for S in lonely_sets:
    if not S: continue
    c1_sum = sum(get_poly(s)[0] for s in S)
    c0_sum = sum(get_poly(s)[1] for s in S)
    if c1_sum == 0 and c0_sum > 0:
        sad_integers.add((c0_sum, tuple(S)))

print(sorted(list(sad_integers)))
