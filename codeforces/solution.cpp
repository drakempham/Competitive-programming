#include<bits/stdc++.h>
using namespace std;using L=long long;int T,n,i,p,m;L a[200005],s,r,u,w;vector<int>v;
int main(){for(cin>>T;T--;cout<<'\n'){
cin>>n;for(s=i=0;i<n;)cin>>a[i],s+=a[i++];
for(w=s,r=u=m=i=0;i<n;r+=a[i],u+=abs(a[i]),i++)if(a[i]>0&&u+s-r-2*a[i]>w)w=u+s-r-2*a[i],m=i+1;
for(v.clear(),p=0,i=m-1;i-->0;)((a[i]>0)^p)&&(v.push_back(i+1),p^=1);if(m)v.push_back(m);
cout<<v.size()<<'\n';for(int x:v)cout<<x<<' ';
}}