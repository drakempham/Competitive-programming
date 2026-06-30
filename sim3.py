def check_stones(n, limit=1000):
    stones = {0: n}
    for i in range(limit):
        m = stones.get(i, 0)
        move = m // 2
        if move > 0:
            stones[i+1] = stones.get(i+1, 0) + move
            stones[i+3] = stones.get(i+3, 0) + move
    print(stones[limit-1], stones[limit-2])

check_stones(68, 100)
check_stones(90, 100)
