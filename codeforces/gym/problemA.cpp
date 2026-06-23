#include<bits/stdc++.h>
using namespace std;
int dp[205][205];
int c[205];
int solve_dp(int l, int r) {
    if (l > r) return 0;
    if (l == r) return 1;
    if (dp[l][r] != -1) return dp[l][r];
    
    int res = solve_dp(l + 1, r) + 1;
    for (int k = l + 1; k <= r; ++k) {
        if (c[l] == c[k]) {
                res = min(res, solve_dp(l + 1, k - 1) + solve_dp(k + 1, r));
        }
    }
    
    return dp[l][r] = res;
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    if (cin >> n) {
        for (int i = 0; i < n; ++i) {
            cin >> c[i];
        }
        memset(dp, -1, sizeof(dp));
        cout << solve_dp(0, n - 1) << "\n";
    }
    return 0;
}
