def get_poly(s):
    c1, c0 = 0, 1
    for _ in range(s):
        c1, c0 = c0 - c1, -2*c1
    return c1, c0

def solve_all(k):
    f = [[] for _ in range(k + 1)]
    f[0] = [([], 0, 0)] # list of (subset, sum_c1, sum_c0)
    
    def get_f(idx):
        if idx < 0: return [([], 0, 0)]
        return f[idx]
        
    for i in range(1, k + 1):
        f[i] = list(get_f(i-1))
        val_c1, val_c0 = get_poly(i-1)
        for subset, prev_c1, prev_c0 in get_f(i-3):
            f[i].append((subset + [i-1], prev_c1 + val_c1, prev_c0 + val_c0))
            
    sad = []
    for subset, c1, c0 in f[k]:
        if c1 == 0 and c0 > 0:
            sad.append((c0, subset))
    return sorted(sad)

print("k=14:", solve_all(14))
