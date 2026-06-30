def get_poly(s):
    c1, c0 = 0, 1
    for _ in range(s):
        c1, c0 = c0 - c1, -2*c1
    return c1, c0

def solve(k):
    # dp[i][sum_c1] = sum_c0
    # Actually we need both the count of such sets and the sum of c0.
    # dp[sum_c1] = (count, sum_c0)
    
    # State: we process positions 0 to k-1.
    # To maintain the 'lonely' condition (difference >= 3),
    # we can keep track of the last included position, or just use DP state:
    # dp[i][sum_c1] = (count, sum_c0)
    # where i is the next position we can consider (so we already skipped appropriately).
    
    # dp[i] will store a dictionary: sum_c1 -> (count, sum_c0)
    dp = [{} for _ in range(k + 3)]
    
    # Base case: empty set
    dp[0][0] = (1, 0)
    
    for i in range(k):
        c1, c0 = get_poly(i)
        for cur_c1, (cnt, cur_c0) in dp[i].items():
            # Option 1: do not include i
            if cur_c1 not in dp[i+1]:
                dp[i+1][cur_c1] = (0, 0)
            dp[i+1][cur_c1] = (dp[i+1][cur_c1][0] + cnt, dp[i+1][cur_c1][1] + cur_c0)
            
            # Option 2: include i
            nxt_c1 = cur_c1 + c1
            nxt_c0 = cur_c0 + cnt * c0
            if nxt_c1 not in dp[i+3]:
                dp[i+3][nxt_c1] = (0, 0)
            dp[i+3][nxt_c1] = (dp[i+3][nxt_c1][0] + cnt, dp[i+3][nxt_c1][1] + nxt_c0)

    # Finally, sum all c0 for sum_c1 == 0 across all valid sets.
    # Wait! If we do it this way, we might double count or miss things.
    # Any valid set is formed by including some elements, the last element could be anything < k.
    # To avoid double counting, DP should just be:
    # State: i = index being considered.
    # We can either include i (then next is i+3) or not include i (next is i+1).
    # The sum of all valid sets formed by elements < k is simply found in dp[k], dp[k+1], dp[k+2] 
    # depending on where the last element jumped.
    pass

# Let's write a better DP:
# dp[i][sum_c1] = (count, sum_c0) using elements strictly < i.
# But elements must be lonely.
# Let's define dp[i] as sets using elements from {0..i-1} with NO element >= i-2 in the set?
# A simpler way:
# Let f[i][sum_c1] be the sets using a subset of {0..i-1} that is lonely.
# To compute f[i], we can either NOT include i-1 (so it's just f[i-1])
# OR we include i-1. If we include i-1, we cannot include i-2 or i-3.
# So the remaining elements must be a lonely subset of {0..i-4}.
# This means f[i] = f[i-1] + (f[i-3] with element i-1 added).
def solve2(k):
    # f[i] is a dict: sum_c1 -> [count, sum_c0]
    f = [{} for _ in range(k + 1)]
    f[0] = {0: [1, 0]}
    
    # We need to define f for negative indices as well.
    # For i < 0, f[i] has only the empty set: {0: [1, 0]}
    def get_f(idx):
        if idx < 0: return {0: [1, 0]}
        return f[idx]
        
    for i in range(1, k + 1):
        # Start with f[i-1]
        f[i] = {c1: [cnt, c0] for c1, (cnt, c0) in get_f(i-1).items()}
        
        # Add element i-1 to all sets in f[i-3]
        val_c1, val_c0 = get_poly(i-1)
        for prev_c1, (prev_cnt, prev_c0) in get_f(i-3).items():
            new_c1 = prev_c1 + val_c1
            new_c0 = prev_c0 + prev_cnt * val_c0
            if new_c1 not in f[i]:
                f[i][new_c1] = [0, 0]
            f[i][new_c1][0] += prev_cnt
            f[i][new_c1][1] += new_c0
            
    # Now f[k] contains all lonely subsets of {0..k-1}
    # We want to sum c0 for all sets where sum_c1 == 0 and c0 > 0.
    total = 0
    if 0 in f[k]:
        # wait, the empty set is in f[k] with sum_c1=0, sum_c0=0.
        # So it won't affect the sum.
        # But are there sets with sum_c1=0 and c0 < 0?
        # The problem implies n is a POSITIVE integer.
        # Let's see all values.
        for c1, (cnt, c0) in f[k].items():
            if c1 == 0:
                print(f"c1=0, count={cnt}, sum_c0={c0}")
                total += c0
    return total

print("S(14) =", solve2(14))
print("S(30) =", solve2(30))
