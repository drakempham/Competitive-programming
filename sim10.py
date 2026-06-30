def get_poly(s):
    c1, c0 = 0, 1
    for _ in range(s):
        c1, c0 = c0 - c1, -2*c1
    return c1, c0

vals = {}
for i in range(1024):
    c1, c0 = 0, 0
    for j in range(10):
        if (i >> j) & 1:
            dc1, dc0 = get_poly(j)
            c1 += dc1
            c0 += dc0
    if (c1, c0) in vals:
        print(f"Collision: {i} and {vals[(c1, c0)]}")
    vals[(c1, c0)] = i
print("Done checking collisions.")
