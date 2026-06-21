#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>

using namespace std;

struct Tim {
    chrono::high_resolution_clock::time_point start;
    Tim() { start = chrono::high_resolution_clock::now(); }
    double elapsed() {
        return chrono::duration<double>(chrono::high_resolution_clock::now() - start).count();
    }
};

uint32_t x12() {
    static uint32_t x = 123456789, y = 362436069, z = 521288629, w = 88675123;
    uint32_t t = x ^ (x << 11);
    x = y; y = z; z = w;
    return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

double rnd() { return (x12() * 2.3283064365386963e-10); }

int N, M, K;
string grid[20];
int cell_type[400];
vector<int> empty_cells;

struct Door {
    int u, v;
    int g;
};

struct Switch {
    int u;
    int type;
};

int dist_arr[409600];
int seen[409600];
int current_token = 0;
int q_u[409600];
int q_mask[409600];

int adj[400][4];
int num_adj[400];

int door_map[400][4];
int switch_at[400];

int bfs(const vector<Door>& doors, const vector<Switch>& switches) {
    current_token++;
    
    for (const auto& d : doors) {
        for (int i = 0; i < num_adj[d.u]; ++i) {
            if (adj[d.u][i] == d.v) door_map[d.u][i] = d.g;
        }
        for (int i = 0; i < num_adj[d.v]; ++i) {
            if (adj[d.v][i] == d.u) door_map[d.v][i] = d.g;
        }
    }
    for (const auto& s : switches) {
        switch_at[s.u] = s.type;
    }

    auto teardown = [&]() {
        for (const auto& d : doors) {
            for (int i = 0; i < num_adj[d.u]; ++i) {
                if (adj[d.u][i] == d.v) door_map[d.u][i] = -1;
            }
            for (int i = 0; i < num_adj[d.v]; ++i) {
                if (adj[d.v][i] == d.u) door_map[d.v][i] = -1;
            }
        }
        for (const auto& s : switches) {
            switch_at[s.u] = -1;
        }
    };

    int head = 0, tail = 0;
    q_u[tail] = 0;
    q_mask[tail] = 0;
    tail++;
    
    seen[0] = current_token;
    dist_arr[0] = 0;

    int target_u = 399;

    while (head < tail) {
        int u = q_u[head];
        int mask = q_mask[head];
        head++;
        int u_idx = (u << 10) | mask;
        int d = dist_arr[u_idx];

        if (u == target_u) {
            teardown();
            return d;
        }

        if (switch_at[u] != -1) {
            int new_mask = mask ^ (1 << switch_at[u]);
            int new_idx = (u << 10) | new_mask;
            if (seen[new_idx] != current_token) {
                seen[new_idx] = current_token;
                dist_arr[new_idx] = d + 1; 
                q_u[tail] = u;
                q_mask[tail] = new_mask;
                tail++;
            }
        }

        for (int i = 0; i < num_adj[u]; ++i) {
            int v = adj[u][i];
            int g = door_map[u][i];
            bool can_pass = true;
            
            if (g != -1) {
                if ( (((g & 1) ^ 1) ^ ((mask >> (g >> 1)) & 1)) == 0 ) {
                    can_pass = false;
                }
            }

            if (can_pass) {
                int v_idx = (v << 10) | mask;
                if (seen[v_idx] != current_token) {
                    seen[v_idx] = current_token;
                    dist_arr[v_idx] = d + 1; 
                    q_u[tail] = v;
                    q_mask[tail] = mask;
                    tail++;
                }
            }
        }
    }
    
    teardown();
    return -1;
}

int plain_bfs_dist[400];
void plain_bfs(int start) {
    for (int i = 0; i < 400; ++i) plain_bfs_dist[i] = -1;
    int q[400];
    int h = 0, t = 0;
    q[t++] = start;
    plain_bfs_dist[start] = 0;
    
    while(h < t) {
        int u = q[h++];
        for (int i = 0; i < num_adj[u]; ++i) {
            int v = adj[u][i];
            if (plain_bfs_dist[v] == -1) {
                plain_bfs_dist[v] = plain_bfs_dist[u] + 1;
                q[t++] = v;
            }
        }
    }
}

void solve() {
    Tim timer;
    if (!(cin >> N >> M >> K)) return;
    
    for (int i = 0; i < 400; ++i) {
        switch_at[i] = -1;
        for (int j = 0; j < 4; ++j) door_map[i][j] = -1;
    }
    
    for (int i = 0; i < N; ++i) {
        cin >> grid[i];
        for (int j = 0; j < N; ++j) {
            if (grid[i][j] == '.') {
                cell_type[i * N + j] = 0;
                empty_cells.push_back(i * N + j);
            } else {
                cell_type[i * N + j] = 1;
            }
        }
    }

    for (int u = 0; u < 400; ++u) {
        num_adj[u] = 0;
        if (cell_type[u] == 1) continue;
        int r = u / 20, c = u % 20;
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr >= 0 && nr < N && nc >= 0 && nc < N) {
                int v = nr * N + nc;
                if (cell_type[v] == 0) {
                    adj[u][num_adj[u]++] = v;
                }
            }
        }
    }

    vector<Door> current_doors;
    vector<Switch> current_switches;
    
    plain_bfs(0);
    int D_target = plain_bfs_dist[399];
    
    if (D_target != -1) {
        int k_idx = 0;
        int init_switch_at[400];
        for (int i = 0; i < 400; ++i) init_switch_at[i] = -1;
        
        for (int L = 1; L < D_target; L += 2) {
            if (current_doors.size() >= M || k_idx >= K) break;
            
            vector<pair<int,int>> cut;
            for (int u = 0; u < 400; ++u) {
                if (plain_bfs_dist[u] == L) {
                    for (int i = 0; i < num_adj[u]; ++i) {
                        int v = adj[u][i];
                        if (plain_bfs_dist[v] == L + 1) cut.push_back({u, v});
                    }
                }
            }
            
            if (cut.size() > 0 && cut.size() <= 5 && current_doors.size() + cut.size() <= M) {
                int target_switch = -1;
                for (int u = 0; u < 400; ++u) {
                    if (plain_bfs_dist[u] <= L && init_switch_at[u] == -1 && cell_type[u] == 0) {
                        target_switch = u;
                        break;
                    }
                }
                
                if (target_switch != -1) {
                    current_switches.push_back({target_switch, k_idx});
                    init_switch_at[target_switch] = k_idx;
                    for (auto e : cut) {
                        current_doors.push_back({e.first, e.second, k_idx * 2 + 1});
                    }
                    k_idx++;
                }
            }
        }
    }

    int current_score = bfs(current_doors, current_switches);
    if(current_score == -1) {
        current_doors.clear();
        current_switches.clear();
        current_score = bfs(current_doors, current_switches);
    }

    int best_score = current_score;
    vector<Door> best_doors = current_doors;
    vector<Switch> best_switches = current_switches;

    double T0 = 5.0; 
    double T1 = 0.01;
    double time_limit = 1.90;
    int iter = 0;

    while (true) {
        if ((iter & 15) == 0) {
            double elapsed = timer.elapsed();
            if (elapsed > time_limit) break;
        }
        iter++;

        vector<Door> next_doors = current_doors;
        vector<Switch> next_switches = current_switches;
        
        int switch_at_local[400];
        for (int i = 0; i < 400; ++i) switch_at_local[i] = -1;
        for (auto s : next_switches) switch_at_local[s.u] = s.type;
        
        int op = x12() % 100;
        bool valid_op = false;
        
        if (op < 25 && next_doors.size() < M) {
            int u = empty_cells[x12() % empty_cells.size()];
            if (num_adj[u] > 0) {
                int v = adj[u][x12() % num_adj[u]];
                bool exists = false;
                for (const auto& d : next_doors) {
                    if ((d.u == u && d.v == v) || (d.u == v && d.v == u)) {
                        exists = true; break;
                    }
                }
                if (!exists) {
                    next_doors.push_back({u, v, (int)(x12() % (2 * K))});
                    valid_op = true;
                }
            }
        } else if (op < 45 && !next_doors.empty()) {
            int idx = x12() % next_doors.size();
            next_doors[idx] = next_doors.back();
            next_doors.pop_back();
            valid_op = true;
        } else if (op < 65) {
            int u = empty_cells[x12() % empty_cells.size()];
            int k = x12() % K;
            bool found = false;
            for(auto& s : next_switches) {
                if(s.u == u) {
                    s.type = k;
                    found = true;
                    break;
                }
            }
            if (!found && next_switches.size() < 30) {
                next_switches.push_back({u, k});
                valid_op = true;
            } else if (found) {
                valid_op = true;
            }
        } else if (op < 75 && !next_switches.empty()) {
            int idx = x12() % next_switches.size();
            next_switches[idx] = next_switches.back();
            next_switches.pop_back();
            valid_op = true;
        } else if (op < 90 && !next_doors.empty()) {
            int idx = x12() % next_doors.size();
            next_doors[idx].g = x12() % (2 * K);
            valid_op = true;
        } else if (op >= 90 && !next_switches.empty()) { 
            int s_idx = x12() % next_switches.size();
            int target_k = next_switches[s_idx].type;
            
            next_switches[s_idx] = next_switches.back();
            next_switches.pop_back();
            
            for (int i = (int)next_doors.size() - 1; i >= 0; --i) {
                if (next_doors[i].g / 2 == target_k) {
                    next_doors[i] = next_doors.back();
                    next_doors.pop_back();
                }
            }
            valid_op = true;
        }
        
        if (!valid_op) continue;
        
        int next_score = bfs(next_doors, next_switches);
        
        if (next_score != -1) {
            double diff = next_score - current_score;
            double temp = T0 * pow(T1 / T0, timer.elapsed() / time_limit);
            if (diff >= 0 || rnd() < exp(diff / temp)) {
                current_score = next_score;
                current_doors = next_doors;
                current_switches = next_switches;
                if (current_score > best_score) {
                    best_score = current_score;
                    best_doors = current_doors;
                    best_switches = current_switches;
                }
            }
        }
    }

    cout << best_doors.size() << "\n";
    for (const auto& d : best_doors) {
        int r1 = d.u / 20, c1 = d.u % 20;
        int r2 = d.v / 20, c2 = d.v % 20;
        int d_a = (c1 == c2) ? 0 : 1; 
        int ia = min(r1, r2);
        int ja = min(c1, c2);
        cout << d_a << " " << ia << " " << ja << " " << d.g << "\n";
    }

    cout << best_switches.size() << "\n";
    for (const auto& s : best_switches) {
        cout << s.u / 20 << " " << s.u % 20 << " " << s.type << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}