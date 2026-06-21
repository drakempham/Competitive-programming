# include <bits/stdc++.h>

using namespace std;

using ll = long long;

const ll MOD = 998244353;
const ll ROOT = 3;
const int MAX_VAL = 200000 + 5;

vector<ll> fact(MAX_VAL + 1), ifact(MAX_VAL + 1);

ll mul(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] % MOD * ifact[n - k] % MOD;
}

ll pw(ll a, ll b) {
    ll r = 1;
    while (b) {
        if (b & 1) r = r * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return r;
}

void prep() {
    fact[0] = 1;

    for (int i = 1; i <= MAX_VAL; i++) {
        ll cur = fact[i - 1] * i;
        fact[i] = cur % MOD;
    }

    ifact[MAX_VAL] = pw(fact[MAX_VAL], MOD - 2);

    for (int i = MAX_VAL; i >= 1; i--) {
        ifact[i - 1] = ifact[i] * i % MOD;
    }
}

void ntt(vector<ll> &a, bool inv) {
    int n = a.size();

    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
            ll wlen = mpow(G, (MOD - 1) / len);
        if (inv) wlen = mpow(wlen, MOD - 2);

        for (int i = 0; i < n; i += len) {
            ll w = 1;

            for (int j = 0; j < len / 2; j++) {
                ll curr_u = a[i + j];
                ll curr_nxt = a[i + j + len / 2] * w % MOD;
                a[i + j] = curr_u + curr_nxt;
                if (a[i + j] >= MOD) a[i + j] -= MOD;

                a[i + j + len / 2] = curr_u - curr_nxt;
                if (a[i + j + len / 2] < 0) a[i + j + len / 2] += MOD;

                w = w * wlen % MOD;
            }
        }
    }
    if (inv) {
        ll iv = mpow(n, MOD - 2);
        for (ll &x : a) x = x * iv % MOD;
    }
}

vector<ll> conv(const vector<ll> &a, const vector<ll> &b) {
        int n = 1;
    if (a.empty() || b.empty()) return {};

        int nd = a.size() + b.size() - 1;

    if (1LL * a.size() * b.size() <= 40000) {
        vector<ll> c(nd);
        for (int i = 0; i < (int)a.size(); i++) {
            for (int j = 0; j < (int)b.size(); j++) {
                c[i + j] = (c[i + j] + a[i] * b[j]) % MOD;
            }
        }

        return c;
    }
    while (n < nd) n <<= 1;

    vector<ll> x(a.begin(), a.end()), y(b.begin(), b.end());

    x.resize(n);
    y.resize(n);
    ntt(x, false);
    ntt(y, false);
    for (int i = 0; i < n; i++) { x[i] = x[i] * y[i] % MOD; }

    ntt(x, true);

    x.resize(nd);
    return x;
}

vector<ll> bp(int n) {
    vector<ll> a(n + 1);

    for (int i = 0; i <= n; i++) {
        a[i] = C(n, i);
    }

    return a;
}

void solve() {
    int n;
    cin >> n;

    vector<int> a(n);
    vector<pair<int, int>> f;

    for (int i = 0; i < n; i++) {
        cin >> a[i];

        if (a[i] != -1) {
            int r = ((i + 1) / 2) & 1;
            f.push_back({i, a[i] ^ r});
        }
    }

    int m = n - 1;
    int sh = 0;
    ll mul = 1;

    vector<vector<ll>> ps;

    auto add = [&](vector<ll> v) {
        int l = 0;

        while (l < (int)v.size() && v[l] == 0) l++;

        sh += l;

        if (l == (int)v.size()) return;

        if (l) v.erase(v.begin(), v.begin() + l);

        while ((int)v.size() > 1 && v.back() == 0) {
            v.pop_back();
        }

        if ((int)v.size() == 1) {
            mul = mul * v[0] % MOD;
        } else {
            ps.push_back(move(v));
        }
    };

    if (f.empty()) {
        mul = 2;
        add(bp(m));
    } else {
        if (f[0].first > 0) {
            add(bp(f[0].first));
        }

        for (int i = 0; i + 1 < (int)f.size(); i++) {
            int l = f[i + 1].first - f[i].first;
            int p = f[i].second == f[i + 1].second ? 0 : 1;

            vector<ll> v(l + 1);

            for (int j = 0; j <= l; j++) {
                if (((l - j) & 1) == p) {
                    v[j] = C(l, j);
                }
            }

            add(move(v));
        }

        if (f.back().first < n - 1) {
            add(bp(n - 1 - f.back().first));
        }
    }

    vector<ll> p = {1};


    if (!ps.empty()) {
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        vector<vector<ll>> q;

        q.reserve(ps.size() * 2 + 5);

        for (auto &v : ps) {
            q.push_back(move(v));
            pq.push({(int)q.back().size(), (int)q.size() - 1});
        }

        while (pq.size() > 1) {
            auto x = pq.top();
            pq.pop();

            auto y = pq.top();
            pq.pop();

            vector<ll> z = conv(q[x.second], q[y.second]);
                vector<ll>().swap(q[x.second]);
            vector<ll>().swap(q[y.second]);

                q.push_back(move(z));
            pq.push({(int)q.back().size(), (int)q.size() - 1});
        }

        p = move(q[pq.top().second]);
    }
    ll temp = 0;
    ll ans = 0;

    for (int i = 0; i < (int)p.size(); i++) {
        ans = (ans + p[i] * C(m, i + sh)) % MOD;
    }

    cout << ans * mul % MOD << '\n';
}

int main()
{
    ios_base:: sync_with_stdio(false);
    cin.tie(NULL);

    prep();

    int t;
    cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}