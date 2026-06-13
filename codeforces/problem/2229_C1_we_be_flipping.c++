# include <iostream>
# include<vector>

using namespace std

void solve() {
    int n
    cin >> n

    vector < int > a(n)
    for (int i=0
         i < n
         i++) {
        cin >> a[i]
    }

    int flip = 0
    vector < int > ans

    for (int i=n-1
         i >= 0
         i--) {
        if (flip % 2 == 1) {
            a[i] = -a[i]
        }
        if (a[i] > 0) {
            flip += 1
            ans.push_back(i+1)
        }

    }

    cout << ans.size() << "\n"

    for (int i=0
         i < ans.size()
         i++) {
        cout << ans[i] << " "
    }
    cout << "\n"
}

int main()
{
    ios_base: : sync_with_stdio(false)
    cin.tie(NULL)

    int t
    cin >> t

    while (t--) {
        solve()
    }

    return 0
}
