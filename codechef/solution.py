import sys

input = sys.stdin.readline


def solve():
    t = int(input())
    res = []

    for _ in range(t):
        n = int(input())
        arr = list(map(int, input().split()))
        arr = [0] + arr
        neg_A = [0] * (n + 1)
        for i in range(1, n + 1):
            neg_A[i] = neg_A[i - 1]
            if arr[i] < 0:
                neg_A[i] += arr[i]
        ans = 0
        for i in range(1, n + 1):
            curr_mul = arr[i] * i * (n - i + 1)
            ans += curr_mul

        stay = []
        key_curr = []

        limit = n+1
        for i in range(1, limit):
            if arr[i] > 0:
                stay.append(i)
                key_curr.append(neg_A[i - 1] + arr[i])

        m = len(stay)
        lt = [0] * m
        rt = [n + 1] * m

        st = []
        for i in range(m):
            while st and key_curr[st[-1]] >= key_curr[i]:
                st.pop()
            if st:
                lt[i] = stay[st[-1]]
            st.append(i)
        st = []
        for i in range(m - 1, -1, -1):
            while st and key_curr[st[-1]] > key_curr[i]:
                st.pop()
            if st:
                rt[i] = stay[st[-1]]
            st.append(i)

        pref = [0] * (n + 1)
        for i in range(1, n + 1):
            pref[i] = pref[i - 1] + neg_A[i - 1]

        add = 0

        for i in range(0, m, 1):
            p = stay[i]
            x = key_curr[i]
            lo = lt[i] + 1
            hi = p
            l, r = lo, hi
            last = lo - 1

            while l <= r:
                mid = (l + r) // 2

                if neg_A[mid - 1] > x:
                    last = mid
                    l = mid + 1
                else:
                    r = mid - 1
            if last >= lo:
                cnt = last - lo + 1
                cur = pref[last] - pref[lo - 1]
                add += (rt[i] - p) * (cur - cnt * x)

        res.append(str(ans + 2 * add))

    print("\n".join(res))


solve()
