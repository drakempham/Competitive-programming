# include <bits/stdc++.h>

using namespace std;

using ll = long long;

bool ck(int x, int n, vector<int>&a, vector<int>&b) {
  int c2 = 0;
  int b0 = 0;
  bool last_0 = false;

  for (int i=0;i<n;i++) {
    int curr = (a[i] >=x) + (b[i] >= x);

    if (curr == 2) {
      c2 += 1;
      last_0 = false;
    } else if (curr == 0) {
      if (!last_0) {
        last_0 = true;
        b0 += 1;
      }
    }
  }

  return c2 > b0;
}

void solve() {
  int n;
  cin >> n;

  vector<int> a(n), b(n);
  int min_val =1e9+7, max_val = 0;

  for (int i=0; i< n;i++) {
    cin >>a[i];
    min_val = min(min_val, a[i]);
    max_val = max(max_val, a[i]);
  }

  for (int i = 0; i < n; i++) {
        cin >> b[i];
        max_val = max(max_val, b[i]);
        min_val = min(min_val, b[i]);
  }


  int l = min_val, r = max_val + 1;

  while (l < r) {
    int mid = l + (r - l) / 2;
    if (ck(mid, n,a,b)) {
      l = mid + 1;
    } else {
      r = mid;
    }
  }

  cout << (l-1) << "\n";
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
