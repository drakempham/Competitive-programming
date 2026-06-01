#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <queue>
#include <algorithm>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <random>
#include <cmath>
#include <numeric>
 
using namespace std;

const int INF = 1000000000;

const int dr[4] = {0, 1, 0, -1};
const int dc[4] = {1, 0, -1, 0};

int n, m, step_limit;
vector<string> vw;
vector<string> hw;
vector<pair<int, int>> ball;
vector<pair<int, int>> basket;
vector<vector<vector<bool>>> open_dir;
int row_stride;
int state_count;
vector<vector<int>> graph;

vector<pair<int, int>> points;
map<pair<int, int>, int> pid;
int nP;
vector<vector<int>> point_state;
vector<vector<vector<vector<int>>>> dist;

vector<vector<vector<vector<int>>>> trip_cost;
struct ChoiceInfo {
    int cost;
    int bkd_best;
    int bd_best;
};
vector<vector<vector<ChoiceInfo>>> choice;
int start_pt;
vector<int> ball_pt;
vector<int> basket_pt;

int route_cost(const vector<int>& order) {
    int dp[4];
    int k0 = order[0];
    for (int bkd = 0; bkd < 4; ++bkd) {
        dp[bkd] = trip_cost[0][0][k0][bkd];
    }
    for (size_t idx = 1; idx < order.size(); ++idx) {
        int k = order[idx];
        int prev_k = order[idx - 1];
        int s_idx = prev_k + 1;
        int ndp[4] = {INF, INF, INF, INF};
        for (int sd = 0; sd < 4; ++sd) {
            if (dp[sd] == INF) continue;
            for (int bkd = 0; bkd < 4; ++bkd) {
                int cost = dp[sd] + trip_cost[s_idx][sd][k][bkd];
                if (cost < ndp[bkd]) {
                    ndp[bkd] = cost;
                }
            }
        }
        for (int bkd = 0; bkd < 4; ++bkd) dp[bkd] = ndp[bkd];
    }
    int best = INF;
    for (int bkd = 0; bkd < 4; ++bkd) {
        best = min(best, dp[bkd]);
    }
    return best;
}

pair<vector<int>, vector<int>> restore_dirs(const vector<int>& order) {
    int K = order.size();
    vector<vector<pair<int, int>>> dp_restore(K, vector<pair<int, int>>(4, {INF, -1}));

    int k0 = order[0];
    for (int bkd = 0; bkd < 4; ++bkd) {
        dp_restore[0][bkd] = {trip_cost[0][0][k0][bkd], -1};
    }

    for (int idx = 1; idx < K; ++idx) {
        int k = order[idx];
        int prev_k = order[idx - 1];
        int s_idx = prev_k + 1;
        for (int bkd = 0; bkd < 4; ++bkd) {
            int best_val = INF;
            int best_sd = -1;
            for (int sd = 0; sd < 4; ++sd) {
                int t = dp_restore[idx - 1][sd].first + trip_cost[s_idx][sd][k][bkd];
                if (t < best_val) {
                    best_val = t;
                    best_sd = sd;
                }
            }
            dp_restore[idx][bkd] = {best_val, best_sd};
        }
    }

    int best_val = INF;
    int best_bkd = -1;
    for (int bkd = 0; bkd < 4; ++bkd) {
        if (dp_restore[K - 1][bkd].first < best_val) {
            best_val = dp_restore[K - 1][bkd].first;
            best_bkd = bkd;
        }
    }

    vector<int> bkd_seq(K);
    int curr_bkd = best_bkd;
    for (int idx = K - 1; idx >= 0; --idx) {
        bkd_seq[idx] = curr_bkd;
        curr_bkd = dp_restore[idx][curr_bkd].second;
    }

    vector<int> bd_seq(K);
    for (int idx = 0; idx < K; ++idx) {
        int k = order[idx];
        int si = (idx == 0) ? start_pt : basket_pt[order[idx - 1]];
        int sd = (idx == 0) ? 0 : bkd_seq[idx - 1];
        int bkd = bkd_seq[idx];

        int best_t = INF;
        int best_bd = -1;
        for (int bd = 0; bd < 4; ++bd) {
            int c1 = dist[si][sd][ball_pt[k]][bd];
            int c2 = dist[ball_pt[k]][bd][basket_pt[k]][bkd];
            int t = c1 + c2 + 2;
            if (t < best_t) {
                best_t = t;
                best_bd = bd;
            }
        }
        bd_seq[idx] = best_bd;
    }

    return {bkd_seq, bd_seq};
}

string trace_path(int sr, int sc, int sd, int dst_r, int dst_c, int dd) {
    int ss = sr * row_stride + sc * 4 + sd;
    int ts = dst_r * row_stride + dst_c * 4 + dd;
    if (ss == ts) return "";

    vector<int> curr_dist(state_count, INF);
    vector<int> prv(state_count, -1);
    vector<int> pop(state_count, 0);

    curr_dist[ss] = 0;
    queue<int> q;
    q.push(ss);

    while (!q.empty()) {
        int s = q.front();
        q.pop();
        if (s == ts) break;
        int cd = curr_dist[s];
        int r = s / row_stride;
        int c = (s % row_stride) / 4;
        int d = s % 4;

        if (open_dir[r][c][d]) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            int ns = nr * row_stride + nc * 4 + d;
            if (curr_dist[ns] == INF) {
                curr_dist[ns] = cd + 1;
                prv[ns] = s;
                pop[ns] = 0;
                q.push(ns);
            }
        }
        {
            int ns = r * row_stride + c * 4 + (d + 1) % 4;
            if (curr_dist[ns] == INF) {
                curr_dist[ns] = cd + 1;
                prv[ns] = s;
                pop[ns] = 1;
                q.push(ns);
            }
        }
        {
            int ns = r * row_stride + c * 4 + (d + 3) % 4;
            if (curr_dist[ns] == INF) {
                curr_dist[ns] = cd + 1;
                prv[ns] = s;
                pop[ns] = 2;
                q.push(ns);
            }
        }
    }

    string ops = "";
    int s = ts;
    const string OC = "FRL";
    while (s != ss) {
        ops += OC[pop[s]];
        s = prv[s];
    }
    reverse(ops.begin(), ops.end());
    return ops;
}

string get_doubled_ops(int cnt) {
    if (cnt <= 1) return string(cnt, 'P');

    struct State {
        int cost;
        int pi;
        int pL;
        char act;
    };

    vector<vector<State>> dp_double(cnt + 1, vector<State>(12, {INF, -1, -1, ' '}));
    dp_double[0][0] = {0, -1, -1, ' '};

    for (int i = 0; i <= cnt; ++i) {
        for (int L = 0; L <= 11; ++L) {
            if (dp_double[i][L].cost == INF) continue;
            int cur_cost = dp_double[i][L].cost;
            int sz = 1 << L;

            if (i + sz <= cnt) {
                if (cur_cost + 1 < dp_double[i + sz][L].cost) {
                    dp_double[i + sz][L] = {cur_cost + 1, i, L, 'P'};
                }
            }
            if (L < 11 && i + (sz * 2) <= cnt) {
                if (cur_cost + 4 < dp_double[i + (sz * 2)][L + 1].cost) {
                    dp_double[i + (sz * 2)][L + 1] = {cur_cost + 4, i, L, 'D'};
                }
            }
        }
    }

    int best_cost = INF;
    int best_L = -1;
    for (int L = 0; L <= 11; ++L) {
        if (dp_double[cnt][L].cost < best_cost) {
            best_cost = dp_double[cnt][L].cost;
            best_L = L;
        }
    }

    if (best_cost >= cnt) {
        return string(cnt, 'P');
    }

    string ops = "";
    int cur_i = cnt;
    int cur_L = best_L;
    while (cur_i > 0) {
        State& s = dp_double[cur_i][cur_L];
        if (s.act == 'P') {
            ops += 'P';
        } else if (s.act == 'D') {
            ops += "MPPM";
        }
        cur_i = s.pi;
        cur_L = s.pL;
    }
    reverse(ops.begin(), ops.end());
    return ops;
}

string compressSingle(const string& seq) {
    int sl = seq.length();
    if (sl < 4) return seq;

    int best_saving = 0;
    string_view best_pattern = "";
    int max_len = min(30, sl / 2);

    for (int pl = 2; pl <= max_len; ++pl) {
        unordered_map<string_view, int> counts;
        counts.reserve(sl);
        for (int st = 0; st <= sl - pl; ++st) {
            string_view pat(seq.data() + st, pl);
            counts[pat]++;
        }

        for (auto& [pat, freq] : counts) {
            if (freq < 2) continue;

            int cnt = 0;
            size_t pos = 0;
            while (true) {
                size_t found = seq.find(pat, pos);
                if (found == string::npos) break;
                cnt++;
                pos = found + pl;
            }

            if (cnt < 2) continue;
            int sav = (cnt - 1) * pl - cnt - 1;
            if (sav > best_saving) {
                best_saving = sav;
                best_pattern = pat;
            }
        }
    }

    if (best_saving > 0 && !best_pattern.empty()) {
        size_t fi = seq.find(best_pattern);
        string prefix = seq.substr(0, fi);
        string macro_reg = "M" + string(best_pattern) + "M";
        string remainder = seq.substr(fi + best_pattern.length());

        string new_remainder = "";
        size_t pos = 0;
        while (pos < remainder.length()) {
            size_t found = remainder.find(best_pattern, pos);
            if (found == string::npos) {
                new_remainder += remainder.substr(pos);
                break;
            }
            new_remainder += remainder.substr(pos, found - pos);
            new_remainder += "P";
            pos = found + best_pattern.length();
        }

        string compressed = prefix + macro_reg + new_remainder;

        int total_P = 0;
        for (char c : compressed) if (c == 'P') total_P++;

        if (total_P > 1) {
            size_t first_P = compressed.find('P');
            size_t last_P = compressed.rfind('P');
            if (last_P - first_P + 1 == (size_t)total_P) {
                string doubled_ops = get_doubled_ops(total_P);
                if ((int)doubled_ops.length() < total_P) {
                    compressed = compressed.substr(0, first_P) + doubled_ops + compressed.substr(last_P + 1);
                }
            }
        }

        return compressed;
    }

    return seq;
}

string compress(const string& seq, const vector<int>& bounds_idx) {
    string best_compressed = compressSingle(seq);
    int K = bounds_idx.size();
    if (K < 3) return best_compressed;

    vector<int> candidates;
    vector<double> fractions = {0.25, 0.33, 0.50, 0.67, 0.75};
    for (double f : fractions) {
        int idx = round(f * (K - 1));
        if (idx >= 1 && idx < K - 1) {
            candidates.push_back(bounds_idx[idx]);
        }
    }
    sort(candidates.begin(), candidates.end());
    candidates.erase(unique(candidates.begin(), candidates.end()), candidates.end());

    for (int split_pt : candidates) {
        string left = seq.substr(0, split_pt);
        string right = seq.substr(split_pt);

        string comp_left = compressSingle(left);
        string comp_right = compressSingle(right);

        string combined = comp_left + comp_right;
        if (combined.length() < best_compressed.length()) {
            best_compressed = combined;
        }
    }

    return best_compressed;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto t_start = chrono::steady_clock::now();

    if (!(cin >> n >> m >> step_limit)) return 0;

    vw.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> vw[i];
    }
    hw.resize(n - 1);
    for (int i = 0; i < n - 1; ++i) {
        cin >> hw[i];
    }

    ball.resize(m);
    basket.resize(m);
    for (int i = 0; i < m; ++i) {
        cin >> ball[i].first >> ball[i].second >> basket[i].first >> basket[i].second;
    }

    open_dir.assign(n, vector<vector<bool>>(n, vector<bool>(4, false)));
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            if (c + 1 < n && vw[r][c] == '0') open_dir[r][c][0] = true;
            if (r + 1 < n && hw[r][c] == '0') open_dir[r][c][1] = true;
            if (c > 0 && vw[r][c - 1] == '0') open_dir[r][c][2] = true;
            if (r > 0 && hw[r - 1][c] == '0') open_dir[r][c][3] = true;
        }
    }

    row_stride = n * 4;
    state_count = n * row_stride;

    graph.resize(state_count);
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            int base = r * row_stride + c * 4;
            for (int d = 0; d < 4; ++d) {
                int s = base + d;
                if (open_dir[r][c][d]) {
                    graph[s].push_back((r + dr[d]) * row_stride + (c + dc[d]) * 4 + d);
                }
                graph[s].push_back(base + (d + 1) % 4);
                graph[s].push_back(base + (d + 3) % 4);
            }
        }
    }

    points.push_back({0, 0});
    for (int k = 0; k < m; ++k) {
        points.push_back(ball[k]);
        points.push_back(basket[k]);
    }
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    nP = points.size();
    for (int i = 0; i < nP; ++i) {
        pid[points[i]] = i;
    }

    point_state.assign(nP, vector<int>(4));
    for (int i = 0; i < nP; ++i) {
        for (int d = 0; d < 4; ++d) {
            point_state[i][d] = points[i].first * row_stride + points[i].second * 4 + d;
        }
    }

    dist.assign(nP, vector<vector<vector<int>>>(4, vector<vector<int>>(nP, vector<int>(4, INF))));
    for (int si = 0; si < nP; ++si) {
        for (int sd = 0; sd < 4; ++sd) {
            vector<int> curr_dist(state_count, INF);
            int start = point_state[si][sd];
            curr_dist[start] = 0;
            queue<int> q;
            q.push(start);
            while (!q.empty()) {
                int s = q.front();
                q.pop();
                int cd = curr_dist[s];
                for (int ns : graph[s]) {
                    if (curr_dist[ns] == INF) {
                        curr_dist[ns] = cd + 1;
                        q.push(ns);
                    }
                }
            }
            for (int di = 0; di < nP; ++di) {
                for (int dd = 0; dd < 4; ++dd) {
                    dist[si][sd][di][dd] = curr_dist[point_state[di][dd]];
                }
            }
        }
    }

    start_pt = pid[{0, 0}];
    ball_pt.resize(m);
    basket_pt.resize(m);
    for (int k = 0; k < m; ++k) {
        ball_pt[k] = pid[ball[k]];
        basket_pt[k] = pid[basket[k]];
    }

    trip_cost.assign(m + 1, vector<vector<vector<int>>>(4, vector<vector<int>>(m, vector<int>(4, INF))));
    for (int s_idx = 0; s_idx <= m; ++s_idx) {
        int si_pt = (s_idx == 0) ? start_pt : basket_pt[s_idx - 1];
        for (int sd = 0; sd < 4; ++sd) {
            for (int k = 0; k < m; ++k) {
                for (int bkd = 0; bkd < 4; ++bkd) {
                    int best = INF;
                    for (int bd = 0; bd < 4; ++bd) {
                        int c1 = dist[si_pt][sd][ball_pt[k]][bd];
                        int c2 = dist[ball_pt[k]][bd][basket_pt[k]][bkd];
                        int t = c1 + c2 + 2;
                        if (t < best) {
                            best = t;
                        }
                    }
                    trip_cost[s_idx][sd][k][bkd] = best;
                }
            }
        }
    }

    choice.assign(nP, vector<vector<ChoiceInfo>>(4, vector<ChoiceInfo>(m)));
    for (int s_idx = 0; s_idx <= m; ++s_idx) {
        int si_pt = (s_idx == 0) ? start_pt : basket_pt[s_idx - 1];
        for (int sd = 0; sd < 4; ++sd) {
            for (int k = 0; k < m; ++k) {
                int best = INF;
                int bkd_best = 0;
                int bd_best = 0;
                for (int bkd = 0; bkd < 4; ++bkd) {
                    int val = trip_cost[s_idx][sd][k][bkd];
                    if (val < best) {
                        best = val;
                        bkd_best = bkd;
                    }
                }
                for (int bd = 0; bd < 4; ++bd) {
                    int c1 = dist[si_pt][sd][ball_pt[k]][bd];
                    int c2 = dist[ball_pt[k]][bd][basket_pt[k]][bkd_best];
                    if (c1 + c2 + 2 == best) {
                        bd_best = bd;
                        break;
                    }
                }
                choice[si_pt][sd][k] = {best, bkd_best, bd_best};
            }
        }
    }

    auto nearest_delivery_first = [&]() {
        vector<int> rem(m);
        iota(rem.begin(), rem.end(), 0);
        vector<int> order;
        int si = start_pt;
        int d = 0;
        while (!rem.empty()) {
            int best_k = -1;
            int best_cost = INF;
            int best_idx = -1;
            for (size_t idx = 0; idx < rem.size(); ++idx) {
                int k = rem[idx];
                int cost = choice[si][d][k].cost;
                if (cost < best_cost) {
                    best_cost = cost;
                    best_k = k;
                    best_idx = idx;
                }
            }
            d = choice[si][d][best_k].bkd_best;
            order.push_back(best_k);
            si = basket_pt[best_k];
            rem.erase(rem.begin() + best_idx);
        }
        return order;
    };

    auto nearest_pickup_first = [&]() {
        vector<int> rem(m);
        iota(rem.begin(), rem.end(), 0);
        vector<int> order;
        int si = start_pt;
        int d = 0;
        while (!rem.empty()) {
            int best_k = -1;
            int best_cost = INF;
            int best_idx = -1;
            for (size_t idx = 0; idx < rem.size(); ++idx) {
                int k = rem[idx];
                int min_pickup = INF;
                for (int bd = 0; bd < 4; ++bd) {
                    min_pickup = min(min_pickup, dist[si][d][ball_pt[k]][bd]);
                }
                if (min_pickup < best_cost) {
                    best_cost = min_pickup;
                    best_k = k;
                    best_idx = idx;
                }
            }
            d = choice[si][d][best_k].bkd_best;
            order.push_back(best_k);
            si = basket_pt[best_k];
            rem.erase(rem.begin() + best_idx);
        }
        return order;
    };

    vector<int> o1 = nearest_delivery_first();
    vector<int> o2 = nearest_pickup_first();
    int c1 = route_cost(o1);
    int c2 = route_cost(o2);
    vector<int> best_order = (c1 <= c2) ? o1 : o2;
    int best_cost = min(c1, c2);

    if (m <= 15) {
        int num_states = (1 << m) * m;
        vector<int> dp_c(num_states, INF);
        vector<int> dp_d(num_states, 0);
        vector<int> dp_p(num_states, -1);

        for (int k = 0; k < m; ++k) {
            ChoiceInfo info = choice[start_pt][0][k];
            int c = info.cost;
            int bkd = info.bkd_best;
            int idx = (1 << k) * m + k;
            dp_c[idx] = c;
            dp_d[idx] = bkd;
        }

        for (int mask = 1; mask < (1 << m); ++mask) {
            for (int last = 0; last < m; ++last) {
                if (!(mask & (1 << last))) continue;
                int idx = mask * m + last;
                if (dp_c[idx] == INF) continue;
                int csf = dp_c[idx];
                int d = dp_d[idx];
                int si = basket_pt[last];
                for (int nxt = 0; nxt < m; ++nxt) {
                    if (mask & (1 << nxt)) continue;
                    ChoiceInfo info = choice[si][d][nxt];
                    int c = info.cost;
                    int nd = info.bkd_best;
                    int nc = csf + c;
                    int nidx = (mask | (1 << nxt)) * m + nxt;
                    if (nc < dp_c[nidx]) {
                        dp_c[nidx] = nc;
                        dp_d[nidx] = nd;
                        dp_p[nidx] = last;
                    }
                }
            }
        }

        int dp_best = INF;
        int dp_last = -1;
        int full_mask = (1 << m) - 1;
        for (int k = 0; k < m; ++k) {
            int idx = full_mask * m + k;
            if (dp_c[idx] < dp_best) {
                dp_best = dp_c[idx];
                dp_last = k;
            }
        }

        if (dp_best != INF) {
            vector<int> order_dp;
            int mask = full_mask;
            int k = dp_last;
            while (k != -1) {
                order_dp.push_back(k);
                int idx = mask * m + k;
                int pk = dp_p[idx];
                mask ^= (1 << k);
                k = pk;
            }
            reverse(order_dp.begin(), order_dp.end());
            int dp_exact = route_cost(order_dp);
            if (dp_exact < best_cost) {
                best_order = order_dp;
                best_cost = dp_exact;
            }
        }
    }

    vector<int> cur_order = best_order;
    int cur_cost = best_cost;

    double time_limit = 1.48;
    mt19937 rng(42);
    uniform_int_distribution<int> dist_m(0, m - 1);
    uniform_real_distribution<double> dist_u(0.0, 1.0);

    long long iter = 0;
    while (true) {
        if ((iter & 4095) == 0) {
            auto t_now = chrono::steady_clock::now();
            double elapsed = chrono::duration<double>(t_now - t_start).count();
            if (elapsed >= time_limit) break;
        }
        iter++;

        auto t_now = chrono::steady_clock::now();
        double elapsed = chrono::duration<double>(t_now - t_start).count();
        double progress = min(elapsed / time_limit, 1.0);
        double temp = max(best_cost * 0.15 * (1.0 - progress), 0.01);

        int move_type = rng() % 3;
        vector<int> new_order = cur_order;
        if (move_type == 0) {
            int i = dist_m(rng);
            int j = dist_m(rng);
            if (i == j) continue;
            swap(new_order[i], new_order[j]);
        } else if (move_type == 1) {
            int i = dist_m(rng);
            int j = dist_m(rng);
            if (i == j) continue;
            if (i > j) swap(i, j);
            reverse(new_order.begin() + i, new_order.begin() + j + 1);
        } else {
            int i = dist_m(rng);
            int j = rng() % (m + 1);
            if (j == i || j == i + 1) continue;
            int elem = new_order[i];
            new_order.erase(new_order.begin() + i);
            int insert_pos = j;
            if (j > i) insert_pos--;
            new_order.insert(new_order.begin() + insert_pos, elem);
        }

        int new_cost = route_cost(new_order);
        int delta = new_cost - cur_cost;
        if (delta < 0 || dist_u(rng) < exp(-delta / temp)) {
            cur_order = new_order;
            cur_cost = new_cost;
            if (cur_cost < best_cost) {
                best_order = cur_order;
                best_cost = cur_cost;
            }
        }
    }

    vector<int> order = best_order;

    string raw = "";
    pair<int, int> pos = {0, 0};
    int d = 0;
    int used = 0;

    auto [bkd_seq, bd_seq] = restore_dirs(order);
    vector<int> bounds_idx = {0};
    int current_len = 0;

    for (size_t idx = 0; idx < order.size(); ++idx) {
        int k = order[idx];
        int sd = d;
        int bd = bd_seq[idx];
        int bkd = bkd_seq[idx];

        int si_pt = pid[pos];
        int c1 = dist[si_pt][sd][ball_pt[k]][bd];
        int c2 = dist[ball_pt[k]][bd][basket_pt[k]][bkd];
        int step_cost = c1 + c2 + 2;

        if (used + step_cost > step_limit) {
            continue;
        }

        string p1 = trace_path(pos.first, pos.second, sd, ball[k].first, ball[k].second, bd);
        string p2 = trace_path(ball[k].first, ball[k].second, bd, basket[k].first, basket[k].second, bkd);

        raw += p1;
        raw += 'S';
        raw += p2;
        raw += 'S';

        current_len += p1.length() + 1 + p2.length() + 1;
        bounds_idx.push_back(current_len);

        used += step_cost;
        pos = basket[k];
        d = bkd;
    }

    string compressed = compress(raw, bounds_idx);

    string output = (compressed.length() <= (size_t)step_limit) ? compressed : raw.substr(0, step_limit);

    for (char c : output) {
        cout << c << "\n";
    }

    return 0;
}
