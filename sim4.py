def get_A_alpha(n):
    stones = {0: n}
    singletons = []
    limit = 200
    for i in range(limit):
        m = stones.get(i, 0)
        rem = m % 2
        move = m // 2
        if rem == 1:
            singletons.append(i)
        if move > 0:
            stones[i+1] = stones.get(i+1, 0) + move
            stones[i+3] = stones.get(i+3, 0) + move
    
    # Calculate A(x) mod x^2+x+2
    c1, c0 = 0, 0
    for s in singletons:
        # compute x^s mod x^2+x+2
        # using a simple iterative approach
        curr1, curr0 = 0, 1 # curr = curr1*x + curr0
        for _ in range(s):
            # multiply by x: curr1*x^2 + curr0*x
            # = curr1*(-x-2) + curr0*x = (curr0 - curr1)*x - 2*curr1
            curr1, curr0 = curr0 - curr1, -2*curr1
        c1 += curr1
        c0 += curr0
    return c1, c0

print(get_A_alpha(68))
print(get_A_alpha(90))
print(get_A_alpha(1))
