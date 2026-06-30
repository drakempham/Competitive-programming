#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

long long c1_arr[80];
long long c0_arr[80];

struct State {
    long long c1, c0;
    bool operator<(const State& other) const {
        if (c1 != other.c1) return c1 < other.c1;
        return c0 < other.c0;
    }
};

vector<State> L_list[3];
vector<State> R_list[3];

void dfs_L(int i, int limit, long long sum_c1, long long sum_c0, int mask) {
    if (i >= limit) {
        L_list[mask].push_back({sum_c1, sum_c0});
        return;
    }
    dfs_L(i + 1, limit, sum_c1, sum_c0, mask);
    
    int new_mask = mask;
    if (i == limit - 2) new_mask = 1;
    if (i == limit - 1) new_mask = 2;
    dfs_L(i + 3, limit, sum_c1 + c1_arr[i], sum_c0 + c0_arr[i], new_mask);
}

void dfs_R(int i, int limit, long long sum_c1, long long sum_c0, int mask, int mid) {
    if (i >= limit) {
        R_list[mask].push_back({sum_c1, sum_c0});
        return;
    }
    dfs_R(i + 1, limit, sum_c1, sum_c0, mask, mid);
    
    int new_mask = mask;
    if (mask == 0) {
        if (i == mid) new_mask = 1;
        else if (i == mid + 1) new_mask = 2;
    }
    dfs_R(i + 3, limit, sum_c1 + c1_arr[i], sum_c0 + c0_arr[i], new_mask, mid);
}

long long solve(int k) {
    int mid = k / 2;
    for (int i=0; i<3; i++) {
        L_list[i].clear();
        R_list[i].clear();
    }
    dfs_L(0, mid, 0, 0, 0);
    dfs_R(mid, k, 0, 0, 0, mid);
    
    for (int i = 0; i < 3; i++) {
        sort(R_list[i].begin(), R_list[i].end());
    }
    
    long long total_sum = 0;
    
    auto process = [&](int l_mask, int r_mask) {
        for (const auto& l : L_list[l_mask]) {
            long long target_c1 = -l.c1;
            State target_state = {target_c1, -2000000000000000000LL};
            auto it = lower_bound(R_list[r_mask].begin(), R_list[r_mask].end(), target_state);
            
            while (it != R_list[r_mask].end() && it->c1 == target_c1) {
                if (l.c0 + it->c0 > 0) {
                    total_sum += l.c0 + it->c0;
                }
                it++;
            }
        }
    };
    
    process(0, 0); process(0, 1); process(0, 2);
    process(1, 0); process(1, 2);
    process(2, 0);
    
    return total_sum;
}

int main() {
    c1_arr[0] = 0; c0_arr[0] = 1;
    for (int s = 1; s < 80; s++) {
        c1_arr[s] = c0_arr[s-1] - c1_arr[s-1];
        c0_arr[s] = -2 * c1_arr[s-1];
    }
    
    cout << "S(14) = " << solve(14) << endl;
    cout << "S(30) = " << solve(30) << endl;
    cout << "S(80) = " << solve(80) << endl;
    return 0;
}
