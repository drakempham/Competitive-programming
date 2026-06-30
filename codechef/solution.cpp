#include<bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n;
    string a, b;
    cin >> n >> a >> b;

    string x, y;

    for (int i = 0; i + 1 < n; i++) {
        if (a[i] != a[i + 1]) x += a[i];
        if (b[i] != b[i + 1]) y += b[i];
    }

    if (x.empty()) {
        cout << (a == b ? "Yes" : "No") << '\n';
        return;
    }

    int j = 0;
    for (char c : x) {
        if (j < (int)y.size() && c == y[j]) j++;
    }

    cout << (j == (int)y.size() ? "Yes" : "No") << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;
    while (t--) solve();

    return 0;
}