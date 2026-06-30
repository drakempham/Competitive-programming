#include<bits/stdc++.h>
using namespace std;

using ll=long long;

void solve() {
    int n;
    cin>>n;
    vector<ll>a(n),b(n);
    for(int i=0;i<n;i++) cin>>a[i];
    for(int i=0;i<n;i++) cin>>b[i];
    vector<vector<int>>v(n+1);
    for(int i=0;i<n;i++) {
        int p=lower_bound(b.begin(),b.end(),a[i])-b.begin()+1;
            if(p>n) {
            cout<<-1<<'\n';
            return;
        }
            v[p].push_back(i+1);
    }


    priority_queue<int,vector<int>,greater<int>>pq;
    vector<int>bit(n+1);
    auto add=[&](int i,int x) {
        for(;i<=n;i+=i&-i) bit[i]+=x;
    };
    auto sum=[&](int i) {
        int r=0;
        for(;i;i-=i&-i) {
            r+=bit[i];
        }
        return r;
    };
    for(int i=1;i<=n;i++) add(i,1);
    ll res=0;
    for(int j=1;j<=n;j++) {
        for(int x:v[j]) pq.push(x);
        if(pq.empty()) {
            cout<<-1<<'\n';
            return;
        }
        int id=pq.top();
        pq.pop();
        res+=sum(id-1);
        add(id,-1);
    }
    cout<<res<<'\n';
}
int main() {
    ios_base:: sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}