# include <bits/stdc++.h>

using namespace std;

using ll = long long;

void solve() {
  int n;
  cin >> n;

  int mn = 1001;
  int mx = 0;
  for (int i=0; i< n; i++) {
    int x;
    cin >>x;
    mn = min(x, mn);
    mx = max(x, mx);
  }

  cout  << (mx-mn + 1) /2 << "\n";
}

int main()
{
    ios_base:: sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}
