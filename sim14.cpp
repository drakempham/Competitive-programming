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

void dfs_L(int i, long long sum_c1, long long sum_c0, int mask) {
    if (i >= 40) {
        L_list[mask].push_back({sum_c1, sum_c0});
        return;
    }
    dfs_L(i + 1, sum_c1, sum_c0, mask);
    
    int new_mask = mask;
    if (i == 38) new_mask = 1;
    if (i == 39) new_mask = 2;
    dfs_L(i + 3, sum_c1 + c1_arr[i], sum_c0 + c0_arr[i], new_mask);
}

void dfs_R(int i, long long sum_c1, long long sum_c0, int mask) {
    if (i >= 80) {
        R_list[mask].push_back({sum_c1, sum_c0});
        return;
    }
    dfs_R(i + 1, sum_c1, sum_c0, mask);
    
    int new_mask = mask;
    if (mask == 0) {
        if (i == 40) new_mask = 1;
        else if (i == 41) new_mask = 2;
    }
    dfs_R(i + 3, sum_c1 + c1_arr[i], sum_c0 + c0_arr[i], new_mask);
}

int main() {
    c1_arr[0] = 0;
    c0_arr[0] = 1;
    for (int s = 1; s < 80; s++) {
        c1_arr[s] = c0_arr[s-1] - c1_arr[s-1];
        c0_arr[s] = -2 * c1_arr[s-1];
    }
    
    dfs_L(0, 0, 0, 0);
    dfs_R(40, 0, 0, 0);
    
    for (int i = 0; i < 3; i++) {
        sort(R_list[i].begin(), R_list[i].end());
    }
    
    long long total_sum = 0;
    
    auto process = [&](int l_mask, int r_mask) {
        for (const auto& l : L_list[l_mask]) {
            long long target_c1 = -l.c1;
            
            // Binary search in R_list[r_mask]
            State target_state = {target_c1, -2000000000000000000LL}; // minimal c0
            auto it = lower_bound(R_list[r_mask].begin(), R_list[r_mask].end(), target_state);
            
            while (it != R_list[r_mask].end() && it->c1 == target_c1) {
                if (l.c0 + it->c0 > 0) {
                    total_sum += l.c0 + it->c0;
                }
                it++;
            }
        }
    };
    
    // L_mask = 0 pairs with 0, 1, 2
    process(0, 0); process(0, 1); process(0, 2);
    // L_mask = 1 pairs with 0, 2
    process(1, 0); process(1, 2);
    // L_mask = 2 pairs with 0
    process(2, 0);
    
    cout << "S(80) = " << total_sum << endl;
    return 0;
}
