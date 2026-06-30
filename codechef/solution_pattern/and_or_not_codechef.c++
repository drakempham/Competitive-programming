#include<bits/stdc++.h>
using namespace std;

using ll=long long;

const int MOD = 998244353;
const int MAXN = 200005;

ll fact[MAXN], invfact[MAXN];

ll pw(ll a, ll b) {
    ll res = 1;
    while (b) {
        if (b&1) res = res*a % MOD;
        a = a*a % MOD;
        b >>= 1;
    }
    return res;
}

ll C(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * invfact[k] % MOD * invfact[n-k] % MOD;
}

void solve() {
    int n;
    string s;
    cin >> n >> s;

    int b = 1;
    for (int i=1;i<n;i++) {
        if (s[i] != s[i-1]) b++;
    }

    ll ans = 0;
    for(int i=0;i<=b-1;i++) {
        ans = (ans + C(n-1,i)) % MOD;
    }

    for(int i=0;i<=b-2;i++) {
        ans = (ans + C(n-1,i)) % MOD;
    }

    cout << ans << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    fact[0] = 1;
    for(int i=1;i<MAXN;i++) fact[i] = fact[i-1]*i % MOD;

    invfact[MAXN-1] = pw(fact[MAXN-1], MOD-2);
    for(int i=MAXN-2;i>=0;i--) {
        invfact[i] = invfact[i+1] * (i+1) % MOD;
    }

    int t;
    cin >> t;
    while(t--) solve();

    return 0;
}