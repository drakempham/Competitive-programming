def get_poly(s):
    c1, c0 = 0, 1
    for _ in range(s):
        c1, c0 = c0 - c1, -2*c1
    return c1, c0

def find_subset(n):
    for i in range(1048576):
        c1, c0 = 0, 0
        for j in range(20):
            if (i >> j) & 1:
                dc1, dc0 = get_poly(j)
                c1 += dc1
                c0 += dc0
        if c1 == 0 and c0 == n:
            subset = [j for j in range(20) if (i >> j) & 1]
            return subset
    return None

print("6:", find_subset(6))
print("1:", find_subset(1))
print("2:", find_subset(2))
print("3:", find_subset(3))
