#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<long long> mono_temp(2);

vector<long long> chain_accumulate(const vector<long long>& ew) {
    vector<long long> summary_dp(ew.size() + 1, 0);
        vector<int> jg(ew.size(), ew.size());
    vector<int> stk;

    int m_len = ew.size();
    for (int i = m_len - 1; i >= 0; --i) {
        while (!stk.empty() && ew[stk.back()] <= ew[i]) {
            stk.pop_back();
        }
        if (!stk.empty()) {
            jg[i] = stk.back();
        }
        stk.push_back(i);
    }

    for (int i = m_len - 1; i >= 0; --i) {
        summary_dp[i] = (jg[i] - i) * ew[i] + summary_dp[jg[i]];
    }

    return summary_dp;
}

void run_vessels() {
    int v_cnt;
    if (!(cin >> v_cnt)) return;
    vector<long long> h_bar(v_cnt);
    int pivot_idx = 0;
    for (int i = 0; i < v_cnt; ++i) {
        cin >> h_bar[i];
        if (h_bar[i] > h_bar[pivot_idx]) {
            pivot_idx = i;
        }
    }
    vector<long long> lb(v_cnt - 1);
    vector<long long> fluid_last(v_cnt);
    for (int i = 0; i < v_cnt - 1; ++i) {
        lb[i] = h_bar[(pivot_idx + 1 + i) % v_cnt];
    }
    
    vector<long long> dp_l = chain_accumulate(lb);
    reverse(lb.begin(), lb.end());
    vector<long long> dp_r = chain_accumulate(lb);
    
    for (int i = 0; i < v_cnt; ++i) {
        fluid_last[(pivot_idx + 1 + i) % v_cnt] = dp_l[i] + dp_r[v_cnt - 1 - i];
    }
    
    for (int i = 0; i < v_cnt; ++i) {
        cout << fluid_last[i] << (i == v_cnt - 1 ? "" : " ");
    }
    cout << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    if (cin >> t) {
        while (t--) {
            run_vessels();
        }
    }
    return 0;
}