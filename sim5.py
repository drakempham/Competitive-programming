def get_poly(s):
    c1, c0 = 0, 1
    for _ in range(s):
        c1, c0 = c0 - c1, -2*c1
    return c1, c0

for s in [2, 5, 8, 13]:
    print(f"s={s}:", get_poly(s))

print("Sum:", sum(get_poly(s)[0] for s in [2, 5, 8, 13]), sum(get_poly(s)[1] for s in [2, 5, 8, 13]))
