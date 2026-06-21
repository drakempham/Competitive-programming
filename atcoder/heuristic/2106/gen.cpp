// Input generator for AtCoder Heuristic 2106 - Castle Renovation
// Usage: ./gen <seed>
// Matches the exact generation logic described in the problem.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <numeric>
#include <queue>

using namespace std;

int N = 20, M = 50, K = 10;

// Union-Find to check connectivity
struct DSU {
    vector<int> p;
    DSU(int n) : p(n) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        p[a] = b; return true;
    }
    bool same(int a, int b) { return find(a) == find(b); }
};

// BFS-based connectivity check
bool all_empty_connected(vector<vector<int>>& cell, int N) {
    // Find first empty cell
    int start = -1;
    int count = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (cell[i][j] == 0) { 
                if (start == -1) start = i * N + j;
                count++;
            }
    if (count == 0) return true;
    
    vector<bool> vis(N * N, false);
    queue<int> q;
    q.push(start);
    vis[start] = true;
    int reached = 1;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    while (!q.empty()) {
        int u = q.front(); q.pop();
        int r = u / N, c = u % N;
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr >= 0 && nr < N && nc >= 0 && nc < N && !vis[nr*N+nc] && cell[nr][nc] == 0) {
                vis[nr*N+nc] = true;
                reached++;
                q.push(nr*N+nc);
            }
        }
    }
    return reached == count;
}

int main(int argc, char* argv[]) {
    long long seed = 42;
    if (argc > 1) seed = atoll(argv[1]);
    
    mt19937 rng(seed);
    
    // Start with all empty
    vector<vector<int>> cell(N, vector<int>(N, 0)); // 0=empty, 1=obstacle
    
    // Collect all cells except (0,0) and (N-1,N-1)
    vector<int> order;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (!(i == 0 && j == 0) && !(i == N-1 && j == N-1))
                order.push_back(i * N + j);
    
    shuffle(order.begin(), order.end(), rng);
    
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    
    // Phase 1: try to make cells obstacles
    for (int idx : order) {
        int r = idx / N, c = idx % N;
        
        // Count obstacle neighbors (cells outside grid count as obstacles)
        int obs_neighbors = 0;
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= N || nc < 0 || nc >= N || cell[nr][nc] == 1)
                obs_neighbors++;
        }
        
        // If at least 3 neighbors are obstacles, skip
        if (obs_neighbors >= 3) continue;
        
        // Temporarily make obstacle
        cell[r][c] = 1;
        
        // Check connectivity
        if (!all_empty_connected(cell, N)) {
            cell[r][c] = 0; // revert
        }
        // else leave as obstacle
    }
    
    // Phase 2: collect obstacle cells and turn some back to empty
    vector<int> obstacles;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (cell[i][j] == 1)
                obstacles.push_back(i * N + j);
    
    shuffle(obstacles.begin(), obstacles.end(), rng);
    
    int converted = 0;
    for (int idx : obstacles) {
        if (converted >= N) break;
        int r = idx / N, c = idx % N;
        
        // Check if at least one adjacent cell is empty
        bool has_empty_neighbor = false;
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr >= 0 && nr < N && nc >= 0 && nc < N && cell[nr][nc] == 0) {
                has_empty_neighbor = true;
                break;
            }
        }
        
        if (has_empty_neighbor) {
            cell[r][c] = 0;
            converted++;
        }
    }
    
    // Output
    cout << N << "\n" << M << "\n" << K << "\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            cout << (cell[i][j] == 0 ? '.' : '#');
        cout << "\n";
    }
    
    return 0;
}
