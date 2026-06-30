def get_singletons(n, limit=1000):
    stones = {0: n}
    singletons = []
    for i in range(limit):
        m = stones.get(i, 0)
        rem = m % 2
        move = m // 2
        if rem == 1:
            singletons.append(i)
        if move > 0:
            stones[i+1] = stones.get(i+1, 0) + move
            stones[i+3] = stones.get(i+3, 0) + move
    return singletons

print("68:", get_singletons(68, 100))
print("90:", get_singletons(90, 100))
