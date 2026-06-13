# include <bits/stdc++.h>

using namespace std;

using ll = long long;

void solve() {
    int n;
    cin >> n;

    vector <ll> a(n);
    for (int i=0;i < n;i++) {
        cin >> a[i];
    }
    ll total = accumulate(a.begin(), a.end(), 0LL);
    ll ans = total;
    ll prefixSum = 0;
    ll prefixAbsSum = 0;
    int pivot_idx = -1;
    for (int i=0;i < n;i++) {
      if (a[i] > 0) {
          ll suffixSum =  total - prefixSum - a[i];
          ll curr_sum = prefixAbsSum  - a[i] + suffixSum;

          if (ans < curr_sum) {
              ans = curr_sum;
              pivot_idx = i;
          }
      }

      prefixSum += a[i];
      prefixAbsSum += llabs(a[i]);
    }

    if (pivot_idx == -1){
      cout << 0 << "\n\n";
      return;
    }

    vector<int> ops;
    bool flipped_odd= false;
    for (int i=pivot_idx - 1; i>=0;i--) {
      bool currPositive = (a[i] > 0);

      if (flipped_odd) {
        currPositive = !currPositive;
      }

      if (currPositive) {
        ops.push_back(i+1);
        flipped_odd = !flipped_odd;
      }
    }

    ops.push_back(pivot_idx + 1);

    cout << ops.size() <<"\n";
    for (int i =0; i < ops.size();i++) {
      cout << ops[i] << " ";
    }

    cout  << "\n";
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
