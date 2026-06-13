#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    sort(a.begin(), a.end());
    
    int ans = 0;
    
    for (int k = 1; k <= n - 1; k++) {
        long long remain = n - k;
        long long sum = 0;
        bool ok = true;
        
        for (int i = 0; i < k; i++) {
            sum += a[i];
            if (a[i] + i > n - 1) {
                ok = false;
                break;
            }
        }
        
        if (ok && sum <= 1LL * k * remain) {
            ans = k;
        }
    }
    
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}