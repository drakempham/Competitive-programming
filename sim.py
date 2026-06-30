def simulate(n):
    stones = {0: n}
    singletons = set()
    i = 0
    while True:
        if i not in stones:
            if all(k < i for k in stones):
                break
            i += 1
            continue
        m = stones[i]
        move = m // 2
        rem = m % 2
        if rem == 1:
            singletons.add(i)
        elif i in singletons:
            singletons.remove(i)
            
        stones[i] = rem
        if move > 0:
            stones[i+1] = stones.get(i+1, 0) + move
            stones[i+3] = stones.get(i+3, 0) + move
        i += 1
    
    return sorted(list(singletons))

print(simulate(68))
print(simulate(90))
