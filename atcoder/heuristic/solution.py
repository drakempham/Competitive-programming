import sys
import time
import random
import math
from collections import deque


def solve():
    data = sys.stdin.buffer.read().split()
    ptr = 0

    def next_int():
        nonlocal ptr
        val = int(data[ptr])
        ptr += 1
        return val

    def next_str():
        nonlocal ptr
        val = data[ptr].decode()
        ptr += 1
        return val

    n, m, step_limit = next_int(), next_int(), next_int()
    vw = [next_str() for _ in range(n)]
    hw = [next_str() for _ in range(n - 1)]

    ball = [None] * m
    basket = [None] * m
    for k in range(m):
        ball[k] = (next_int(), next_int())
        basket[k] = (next_int(), next_int())

    t0 = time.time()

    dr = [0, 1, 0, -1]
    dc = [1, 0, -1, 0]

    open_dir = [[[False] * 4 for _ in range(n)] for _ in range(n)]
    for r in range(n):
        for c in range(n):
            if c + 1 < n and vw[r][c] == '0':
                open_dir[r][c][0] = True
            if r + 1 < n and hw[r][c] == '0':
                open_dir[r][c][1] = True
            if c > 0 and vw[r][c - 1] == '0':
                open_dir[r][c][2] = True
            if r > 0 and hw[r - 1][c] == '0':
                open_dir[r][c][3] = True

    row_stride = n * 4
    state_count = n * row_stride
    INF = 10 ** 9

    graph = [None] * state_count
    for r in range(n):
        for c in range(n):
            base = r * row_stride + c * 4
            for d in range(4):
                s = base + d
                nb = []
                if open_dir[r][c][d]:
                    nb.append((r + dr[d]) * row_stride + (c + dc[d]) * 4 + d)
                nb.append(base + (d + 1) % 4)
                nb.append(base + (d + 3) % 4)
                graph[s] = nb

    points = list(set([(0, 0)] + list(ball) + list(basket)))
    pid = {p: i for i, p in enumerate(points)}
    pcount = len(points)
    point_state = [[points[i][0] * row_stride + points[i][1]
                    * 4 + d for d in range(4)] for i in range(pcount)]

    dist = [[[[INF] * 4 for _ in range(pcount)]
             for _ in range(4)] for _ in range(pcount)]

    for si in range(pcount):
        for sd in range(4):
            curr_dist = [INF] * state_count
            start = point_state[si][sd]
            curr_dist[start] = 0
            q = deque([start])
            while q:
                s = q.popleft()
                cd = curr_dist[s]
                for ns in graph[s]:
                    if curr_dist[ns] == INF:
                        curr_dist[ns] = cd + 1
                        q.append(ns)
            for di in range(pcount):
                for dd in range(4):
                    dist[si][sd][di][dd] = curr_dist[point_state[di][dd]]

    start_pt = pid[(0, 0)]
    ball_pt = [pid[ball[k]] for k in range(m)]
    basket_pt = [pid[basket[k]] for k in range(m)]

    block16 = 16 * m
    block4 = 4 * m

    trip_cost = [INF] * ((m + 1) * block16)
    for s_idx in range(m + 1):
        si_pt = start_pt if s_idx == 0 else basket_pt[s_idx - 1]
        for sd in range(4):
            sd_base = s_idx * block16 + sd * block4
            for k in range(m):
                k_base = sd_base + k * 4
                for bkd in range(4):
                    best = INF
                    for bd in range(4):
                        c1 = dist[si_pt][sd][ball_pt[k]][bd]
                        c2 = dist[ball_pt[k]][bd][basket_pt[k]][bkd]
                        t = c1 + c2 + 2
                        if t < best:
                            best = t
                    trip_cost[k_base + bkd] = best

    choice = {}
    for s_idx in range(m + 1):
        si_pt = start_pt if s_idx == 0 else basket_pt[s_idx - 1]
        for sd in range(4):
            sd_base = s_idx * block16 + sd * block4
            for k in range(m):
                k_base = sd_base + k * 4
                best = INF
                bkd_best = bd_best = 0
                for bkd in range(4):
                    val = trip_cost[k_base + bkd]
                    if val < best:
                        best = val
                        bkd_best = bkd
                for bd in range(4):
                    c1 = dist[si_pt][sd][ball_pt[k]][bd]
                    c2 = dist[ball_pt[k]][bd][basket_pt[k]][bkd_best]
                    if c1 + c2 + 2 == best:
                        bd_best = bd
                        break
                choice[(si_pt, sd, k)] = (best, bkd_best, bd_best)

    def route_cost(order):
        dp = [INF] * 4
        k0 = order[0]
        base0 = k0 * 4
        dp[0] = trip_cost[base0]
        dp[1] = trip_cost[base0 + 1]
        dp[2] = trip_cost[base0 + 2]
        dp[3] = trip_cost[base0 + 3]

        for idx in range(1, len(order)):
            k = order[idx]
            prev_k = order[idx - 1]
            sidx_base = (prev_k + 1) * block16

            d0, d1, d2, d3 = dp
            nd0 = nd1 = nd2 = nd3 = INF

            sd_base = sidx_base + k * 4
            t0 = trip_cost[sd_base]
            t1 = trip_cost[sd_base + 1]
            t2 = trip_cost[sd_base + 2]
            t3 = trip_cost[sd_base + 3]
            if d0 + t0 < nd0:
                nd0 = d0 + t0
            if d0 + t1 < nd1:
                nd1 = d0 + t1
            if d0 + t2 < nd2:
                nd2 = d0 + t2
            if d0 + t3 < nd3:
                nd3 = d0 + t3

            sd_base += block4
            t0 = trip_cost[sd_base]
            t1 = trip_cost[sd_base + 1]
            t2 = trip_cost[sd_base + 2]
            t3 = trip_cost[sd_base + 3]
            if d1 + t0 < nd0:
                nd0 = d1 + t0
            if d1 + t1 < nd1:
                nd1 = d1 + t1
            if d1 + t2 < nd2:
                nd2 = d1 + t2
            if d1 + t3 < nd3:
                nd3 = d1 + t3

            sd_base += block4
            t0 = trip_cost[sd_base]
            t1 = trip_cost[sd_base + 1]
            t2 = trip_cost[sd_base + 2]
            t3 = trip_cost[sd_base + 3]
            if d2 + t0 < nd0:
                nd0 = d2 + t0
            if d2 + t1 < nd1:
                nd1 = d2 + t1
            if d2 + t2 < nd2:
                nd2 = d2 + t2
            if d2 + t3 < nd3:
                nd3 = d2 + t3

            sd_base += block4
            t0 = trip_cost[sd_base]
            t1 = trip_cost[sd_base + 1]
            t2 = trip_cost[sd_base + 2]
            t3 = trip_cost[sd_base + 3]
            if d3 + t0 < nd0:
                nd0 = d3 + t0
            if d3 + t1 < nd1:
                nd1 = d3 + t1
            if d3 + t2 < nd2:
                nd2 = d3 + t2
            if d3 + t3 < nd3:
                nd3 = d3 + t3

            dp = [nd0, nd1, nd2, nd3]

        return min(dp)

    def restore_dirs(order):
        K = len(order)
        dp = [[(INF, -1) for _ in range(4)] for _ in range(K)]

        k0 = order[0]
        base0 = k0 * 4
        for bkd in range(4):
            dp[0][bkd] = (trip_cost[base0 + bkd], -1)

        for idx in range(1, K):
            k = order[idx]
            prev_k = order[idx - 1]
            sidx_base = (prev_k + 1) * block16
            for bkd in range(4):
                best_val = INF
                best_sd = -1
                for sd in range(4):
                    t = dp[idx - 1][sd][0] + \
                        trip_cost[sidx_base + sd * block4 + k * 4 + bkd]
                    if t < best_val:
                        best_val = t
                        best_sd = sd
                dp[idx][bkd] = (best_val, best_sd)

        best_val = INF
        best_bkd = -1
        for bkd in range(4):
            if dp[K - 1][bkd][0] < best_val:
                best_val = dp[K - 1][bkd][0]
                best_bkd = bkd

        bkd_seq = [0] * K
        curr_bkd = best_bkd
        for idx in range(K - 1, -1, -1):
            bkd_seq[idx] = curr_bkd
            curr_bkd = dp[idx][curr_bkd][1]

        bd_seq = [0] * K
        for idx in range(K):
            k = order[idx]
            si = start_pt if idx == 0 else basket_pt[order[idx - 1]]
            sd = 0 if idx == 0 else bkd_seq[idx - 1]
            bkd = bkd_seq[idx]

            best_t = INF
            best_bd = -1
            for bd in range(4):
                c1 = dist[si][sd][ball_pt[k]][bd]
                c2 = dist[ball_pt[k]][bd][basket_pt[k]][bkd]
                t = c1 + c2 + 2
                if t < best_t:
                    best_t = t
                    best_bd = bd
            bd_seq[idx] = best_bd

        return bkd_seq, bd_seq

    def nearest_delivery_first():
        rem = list(range(m))
        order = []
        si = start_pt
        d = 0
        while rem:
            bk = min(rem, key=lambda k: choice[(si, d, k)][0])
            _, d, _ = choice[(si, d, bk)]
            order.append(bk)
            si = basket_pt[bk]
            rem.remove(bk)
        return order

    def nearest_pickup_first():
        rem = list(range(m))
        order = []
        si = start_pt
        d = 0
        while rem:
            bk = min(rem, key=lambda k: min(
                dist[si][d][ball_pt[k]][bd] for bd in range(4)))
            _, d, _ = choice[(si, d, bk)]
            order.append(bk)
            si = basket_pt[bk]
            rem.remove(bk)
        return order

    o1, o2 = nearest_delivery_first(), nearest_pickup_first()
    c1, c2 = route_cost(o1), route_cost(o2)
    best_order = o1[:] if c1 <= c2 else o2[:]
    best_cost = min(c1, c2)

    if m <= 15 and time.time() - t0 < 1.0:
        full = (1 << m) - 1
        sz = m << m
        if sz < 600000:
            dp_c = [INF] * sz
            dp_d = [0] * sz
            dp_p = [-1] * sz

            for k in range(m):
                c, bkd, bd = choice[(start_pt, 0, k)]
                idx = (1 << k) * m + k
                dp_c[idx] = c
                dp_d[idx] = bkd

            for mask in range(1, 1 << m):
                for last in range(m):
                    if not (mask & (1 << last)):
                        continue
                    idx = mask * m + last
                    if dp_c[idx] == INF:
                        continue
                    csf = dp_c[idx]
                    d = dp_d[idx]
                    si = basket_pt[last]
                    for nxt in range(m):
                        if mask & (1 << nxt):
                            continue
                        info = choice.get((si, d, nxt))
                        if info is None:
                            continue
                        c, nd, _ = info
                        nc = csf + c
                        nidx = (mask | (1 << nxt)) * m + nxt
                        if nc < dp_c[nidx]:
                            dp_c[nidx] = nc
                            dp_d[nidx] = nd
                            dp_p[nidx] = last

            dp_best = INF
            dp_last = 0
            for k in range(m):
                idx = full * m + k
                if dp_c[idx] < dp_best:
                    dp_best = dp_c[idx]
                    dp_last = k

            if dp_best != INF:
                order_dp = []
                mask = full
                k = dp_last
                while k != -1:
                    order_dp.append(k)
                    idx = mask * m + k
                    pk = dp_p[idx]
                    mask ^= (1 << k)
                    k = pk
                order_dp.reverse()
                dp_exact = route_cost(order_dp)
                if dp_exact < best_cost:
                    best_order = order_dp
                    best_cost = dp_exact

    rng = random.Random(42)
    cur_order = best_order[:]
    cur_cost = best_cost
    limit = t0 + 1.85

    while time.time() < limit and m > 1:
        progress = (time.time() - t0) / 1.85
        temp = max(best_cost * 0.15 * (1.0 - progress), 0.01)

        move = rng.randint(0, 2)
        if move == 0:
            i = rng.randint(0, m - 1)
            j = rng.randint(0, m - 1)
            if i == j:
                continue
            new_order = cur_order[:]
            new_order[i], new_order[j] = new_order[j], new_order[i]
        elif move == 1:
            i = rng.randint(0, m - 1)
            j = rng.randint(0, m - 1)
            if i == j:
                continue
            if i > j:
                i, j = j, i
            new_order = cur_order[:i] + \
                cur_order[i:j + 1][::-1] + cur_order[j + 1:]
        else:
            i = rng.randint(0, m - 1)
            j = rng.randint(0, m)
            if j == i or j == i + 1:
                continue
            elem = cur_order[i]
            new_order = cur_order[:i] + cur_order[i + 1:]
            if j > i:
                j -= 1
            new_order = new_order[:j] + [elem] + new_order[j:]

        new_cost = route_cost(new_order)
        delta = new_cost - cur_cost
        if delta < 0 or (temp > 0 and rng.random() < math.exp(-delta / temp)):
            cur_order = new_order
            cur_cost = new_cost
            if cur_cost < best_cost:
                best_order = cur_order[:]
                best_cost = cur_cost

    order = best_order

    def trace_path(sr, sc, sd, dst_r, dst_c, dd):
        ss = sr * row_stride + sc * 4 + sd
        ts = dst_r * row_stride + dst_c * 4 + dd
        if ss == ts:
            return ""
        curr_dist = [INF] * state_count
        prv = [-1] * state_count
        pop = [0] * state_count
        curr_dist[ss] = 0
        q = deque([ss])
        while q:
            s = q.popleft()
            if s == ts:
                break
            cd = curr_dist[s]
            r = s // row_stride
            c = (s % row_stride) // 4
            d = s % 4
            if open_dir[r][c][d]:
                nr, nc = r + dr[d], c + dc[d]
                ns = nr * row_stride + nc * 4 + d
                if curr_dist[ns] == INF:
                    curr_dist[ns] = cd + 1
                    prv[ns] = s
                    pop[ns] = 0
                    q.append(ns)
            ns = r * row_stride + c * 4 + (d + 1) % 4
            if curr_dist[ns] == INF:
                curr_dist[ns] = cd + 1
                prv[ns] = s
                pop[ns] = 1
                q.append(ns)
            ns = r * row_stride + c * 4 + (d + 3) % 4
            if curr_dist[ns] == INF:
                curr_dist[ns] = cd + 1
                prv[ns] = s
                pop[ns] = 2
                q.append(ns)
        ops = []
        s = ts
        OC = "FRL"
        while s != ss:
            ops.append(OC[pop[s]])
            s = prv[s]
        ops.reverse()
        return ''.join(ops)

    parts = []
    pos = (0, 0)
    d = 0
    used = 0
    delivered = []

    bkd_seq, bd_seq = restore_dirs(order)

    bounds = [0]
    for idx, k in enumerate(order):
        sd = d
        bd = bd_seq[idx]
        bkd = bkd_seq[idx]

        si_pt = pid[pos]
        c1 = dist[si_pt][sd][ball_pt[k]][bd]
        c2 = dist[ball_pt[k]][bd][basket_pt[k]][bkd]
        step_cost = c1 + c2 + 2

        if used + step_cost > step_limit:
            continue

        p1 = trace_path(pos[0], pos[1], sd, ball[k][0], ball[k][1], bd)
        p2 = trace_path(ball[k][0], ball[k][1], bd,
                        basket[k][0], basket[k][1], bkd)

        parts.append(p1)
        parts.append('S')
        parts.append(p2)
        parts.append('S')

        used += step_cost
        pos = basket[k]
        d = bkd
        delivered.append(k)
        bounds.append(len(p1) + len(p2) + 2)

    raw = ''.join(parts)

    bounds_idx = [0]
    for b in bounds[1:]:
        bounds_idx.append(bounds_idx[-1] + b)

    def pick_macro(seq):
        sl = len(seq)
        if sl < 4:
            return None, 0
        best_saving = 0
        best_pattern = None
        max_len = min(100, sl // 2)

        for pl in range(2, max_len + 1):
            seen = set()
            for st in range(sl - pl + 1):
                pat = seq[st:st + pl]
                if pat in seen:
                    continue
                seen.add(pat)

                cnt = seq.count(pat)
                if cnt < 2:
                    continue
                sav = (cnt - 1) * pl - cnt - 1
                if sav > best_saving:
                    best_saving = sav
                    best_pattern = pat
        return best_pattern, best_saving

    def use_macro(seq, pat):
        fi = seq.find(pat)
        return seq[:fi] + "M" + pat + "M" + seq[fi + len(pat):].replace(pat, "P")

    pat, sav = pick_macro(raw)
    compressed = use_macro(raw, pat) if pat and sav > 0 else raw

    output = compressed if len(compressed) <= step_limit else raw[:step_limit]
    sys.stdout.write('\n'.join(output) + '\n')


if __name__ == '__main__':
    solve()
