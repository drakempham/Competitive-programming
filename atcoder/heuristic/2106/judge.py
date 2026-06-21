#!/usr/bin/env python3
"""
Score evaluator for AtCoder 2106 - Castle Renovation
Given the input (maze) and output (doors/switches placement), compute the score.

Usage: python3 judge.py <input_file> <output_file>
"""

import sys
import math
from collections import deque


def compute_score(input_text, output_text):
    lines_in = input_text.strip().split('\n')
    lines_out = output_text.strip().split('\n')
    
    idx = 0
    N = int(lines_in[idx]); idx += 1
    M = int(lines_in[idx]); idx += 1
    K = int(lines_in[idx]); idx += 1
    
    grid = []
    for i in range(N):
        grid.append(lines_in[idx]); idx += 1
    
    def is_empty(r, c):
        if r < 0 or r >= N or c < 0 or c >= N: return False
        return grid[r][c] == '.'
    
    # Parse output
    oidx = 0
    D = int(lines_out[oidx]); oidx += 1
    
    # door_map: (r1,c1,r2,c2) -> door_type g
    door_map = {}
    
    for _ in range(D):
        parts = lines_out[oidx].split(); oidx += 1
        d, i, j, g = int(parts[0]), int(parts[1]), int(parts[2]), int(parts[3])
        if d == 0:
            # between (i,j) and (i+1,j)
            r1, c1, r2, c2 = i, j, i+1, j
        else:
            # between (i,j) and (i,j+1)
            r1, c1, r2, c2 = i, j, i, j+1
        key = (min(r1*N+c1, r2*N+c2), max(r1*N+c1, r2*N+c2))
        door_map[key] = g
    
    S = int(lines_out[oidx]); oidx += 1
    
    # switch_map: cell -> switch_kind
    switch_map = {}
    
    for _ in range(S):
        parts = lines_out[oidx].split(); oidx += 1
        p, q, s = int(parts[0]), int(parts[1]), int(parts[2])
        switch_map[(p, q)] = s
    
    # BFS: state = (r, c, mask)
    # mask is K-bit integer representing parity of each switch kind pressed
    # door 2k is open when bit k of mask == 0 (even presses)
    # door 2k+1 is open when bit k of mask == 1 (odd presses)
    # door g is open iff: if g is even (g=2k) -> bit k of mask == 0
    #                      if g is odd (g=2k+1) -> bit k of mask == 1
    # => door g is open iff (g % 2) == ((mask >> (g // 2)) & 1)
    
    dr = [-1, 1, 0, 0]
    dc = [0, 0, -1, 1]
    
    INF = float('inf')
    # dist[(r, c, mask)] = steps
    dist = {}
    start = (0, 0, 0)
    dist[start] = 0
    q = deque([start])
    
    target = (N-1, N-1)
    
    while q:
        r, c, mask = q.popleft()
        d = dist[(r, c, mask)]
        
        if (r, c) == target:
            # Found! Compute score
            T = d
            # score = round(10^6 * log2(T / N))
            ratio = T / N
            if ratio <= 1:
                score = 0
            else:
                score = round(1_000_000 * math.log2(ratio))
            return T, score
        
        # Action 1: Press switch (if any on current cell)
        if (r, c) in switch_map:
            k = switch_map[(r, c)]
            new_mask = mask ^ (1 << k)
            state = (r, c, new_mask)
            if state not in dist:
                dist[state] = d + 1
                q.append(state)
        
        # Action 2: Move to adjacent empty cell
        for i in range(4):
            nr, nc = r + dr[i], c + dc[i]
            if not is_empty(nr, nc):
                continue
            
            # Check if there's a door between (r,c) and (nr,nc)
            u = r * N + c
            v = nr * N + nc
            key = (min(u, v), max(u, v))
            
            if key in door_map:
                g = door_map[key]
                k = g >> 1  # g // 2
                parity = (g & 1)  # g % 2
                door_open = (parity == ((mask >> k) & 1))
                if not door_open:
                    continue
            
            state = (nr, nc, mask)
            if state not in dist:
                dist[state] = d + 1
                q.append(state)
    
    # Throne not reachable -> score = 1
    return -1, 1


def main():
    if len(sys.argv) < 3:
        print("Usage: python3 judge.py <input_file> <output_file>")
        sys.exit(1)
    
    with open(sys.argv[1]) as f:
        input_text = f.read()
    with open(sys.argv[2]) as f:
        output_text = f.read()
    
    T, score = compute_score(input_text, output_text)
    if T == -1:
        print(f"T=unreachable, score=1")
    else:
        print(f"T={T}, score={score}")


if __name__ == "__main__":
    main()
