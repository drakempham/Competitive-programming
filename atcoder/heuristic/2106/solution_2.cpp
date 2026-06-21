#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<chrono>
#include<algorithm>
#include<cstdint>
#include<utility>
using namespace std;

struct Ti{
    chrono::high_resolution_clock::time_point s;
    Ti(){s=chrono::high_resolution_clock::now();}
    double e(){return chrono::duration<double>(chrono::high_resolution_clock::now()-s).count();}
};

uint32_t rx(){
    static uint32_t a=123456789,b=362436069,c=521288629,d=88675123;
    uint32_t t=a^(a<<11);a=b;b=c;c=d;
    return d=(d^(d>>19))^(t^(t>>8));
}
int ri(int n){return rx()%n;}
double rf(){return rx()*2.3283064365386963e-10;}

int N,M,K;
string G[20];
int ct[400],ec[400],ne;

struct D{int u,v,g;};
struct S{int u,t;};

D cd[50],bd[50];
int cn,bn;
S cs[400],bs[400];
int cm,bm;

int aj[400][4],na[400];
int da[409600],sn[409600],tk;
int qu[409600],qm[409600];
int dm[400][4],sw[400];
int bp[409600];
int pu[5000],pv[5000];
int np;

template<bool P>
int bf(){
    tk++;
    for(int a=0;a<cn;a++){
        int u=cd[a].u,v=cd[a].v,g=cd[a].g;
        for(int i=0;i<na[u];i++)if(aj[u][i]==v)dm[u][i]=g;
        for(int i=0;i<na[v];i++)if(aj[v][i]==u)dm[v][i]=g;
    }
    for(int a=0;a<cm;a++)sw[cs[a].u]=cs[a].t;
    auto td=[&](){
        for(int a=0;a<cn;a++){
            int u=cd[a].u,v=cd[a].v;
            for(int i=0;i<na[u];i++)if(aj[u][i]==v)dm[u][i]=-1;
            for(int i=0;i<na[v];i++)if(aj[v][i]==u)dm[v][i]=-1;
        }
        for(int a=0;a<cm;a++)sw[cs[a].u]=-1;
    };
    int h=0,t=1;qu[0]=0;qm[0]=0;sn[0]=tk;da[0]=0;
    if(P)bp[0]=-1;
    while(h<t){
        int u=qu[h],mk=qm[h];h++;
        int ui=(u<<10)|mk,d=da[ui];
        if(u==399){
            if(P){np=0;int ci=ui;while(bp[ci]!=-1){int pi=bp[ci],cu=ci>>10,pp=pi>>10;if(cu!=pp&&np<5000){pu[np]=cu<pp?cu:pp;pv[np]=cu<pp?pp:cu;np++;}ci=pi;}}
            td();return d;
        }
        if(sw[u]!=-1){int nm=mk^(1<<sw[u]),ni=(u<<10)|nm;if(sn[ni]!=tk){sn[ni]=tk;da[ni]=d+1;if(P)bp[ni]=ui;qu[t]=u;qm[t]=nm;t++;}}
        for(int i=0;i<na[u];i++){
            int v=aj[u][i],g=dm[u][i];
            if(__builtin_expect(g!=-1,0))if((((g&1)^1)^((mk>>(g>>1))&1))==0)continue;
            int vi=(v<<10)|mk;
            if(sn[vi]!=tk){sn[vi]=tk;da[vi]=d+1;if(P)bp[vi]=ui;qu[t]=v;qm[t]=mk;t++;}
        }
    }
    td();return -1;
}

void sv(){bn=cn;for(int i=0;i<cn;i++)bd[i]=cd[i];bm=cm;for(int i=0;i<cm;i++)bs[i]=cs[i];}

int dfn[400],low[400],timer_b;
bool is_br[400][400];
void dfs_br(int u,int p){
    dfn[u]=low[u]=++timer_b;
    for(int i=0;i<na[u];i++){
        int v=aj[u][i];
        if(v==p)continue;
        if(dfn[v])low[u]=min(low[u],dfn[v]);
        else{
            dfs_br(v,u);
            low[u]=min(low[u],low[v]);
            if(low[v]>dfn[u]){is_br[u][v]=is_br[v][u]=true;}
        }
    }
}

int br_u[400],br_v[400],nb=0;
bool has_0[400],has_399[400];
vector<int> tail_cells[400];

void find_bridges(){
    timer_b=0;
    for(int i=0;i<400;i++){dfn[i]=0;for(int j=0;j<400;j++)is_br[i][j]=false;}
    dfs_br(0,-1);
    nb=0;
    for(int u=0;u<400;u++){
        for(int i=0;i<na[u];i++){
            int v=aj[u][i];
            if(u<v&&is_br[u][v]){
                int q[400],h=0,t=0;bool vis[400]={false};
                q[t++]=v;vis[v]=true;vis[u]=true;
                bool reach0=false;
                if(v==0)reach0=true;
                while(h<t){
                    int cur=q[h++];
                    if(cur==0)reach0=true;
                    for(int j=0;j<na[cur];j++){
                        int nxt=aj[cur][j];
                        if(!vis[nxt]){vis[nxt]=true;q[t++]=nxt;}
                    }
                }
                if(reach0){
                    h=0;t=0;for(int j=0;j<400;j++)vis[j]=false;
                    q[t++]=u;vis[u]=true;vis[v]=true;
                    while(h<t){
                        int cur=q[h++];
                        for(int j=0;j<na[cur];j++){
                            int nxt=aj[cur][j];
                            if(!vis[nxt]){vis[nxt]=true;q[t++]=nxt;}
                        }
                    }
                    br_u[nb]=v;br_v[nb]=u;
                }else{
                    br_u[nb]=u;br_v[nb]=v;
                }
                has_0[nb]=false;has_399[nb]=false;
                tail_cells[nb].clear();
                for(int j=0;j<400;j++)if(vis[j]&&j!=br_u[nb]){
                    tail_cells[nb].push_back(j);
                    if(j==0)has_0[nb]=true;
                    if(j==399)has_399[nb]=true;
                }
                nb++;
            }
        }
    }
}

void bridge_init(){
    cn=0;cm=0;
    vector<int> b399;
    for(int i=0;i<nb;i++)if(has_399[i]&&!has_0[i])b399.push_back(i);
    if(b399.empty())return;
    
    int b_10=b399[ri(b399.size())];
    
    vector<int> cand;
    for(int i=0;i<nb;i++){
        if(i==b_10)continue;
        if(has_0[i]||has_399[i])continue;
        bool ok=true;
        for(int c:tail_cells[i]){
            for(int c2:tail_cells[b_10])if(c==c2){ok=false;break;}
            if(!ok)break;
        }
        if(ok)cand.push_back(i);
    }
    
    int picks[9];
    int pt=0;
    bool used[400]={false};
    
    vector<int> c2=cand;
    for(int i=1;i<(int)c2.size();i++){
        int j=ri(i+1);
        swap(c2[i],c2[j]);
    }
    
    for(int x:c2){
        if(pt==9)break;
        bool ok=true;
        for(int c:tail_cells[x])if(used[c]){ok=false;break;}
        if(ok){
            picks[pt++]=x;
            for(int c:tail_cells[x])used[c]=true;
        }
    }
    
    bool in_tail[400]={false};
    for(int c:tail_cells[b_10])in_tail[c]=true;
    for(int i=0;i<pt;i++)for(int c:tail_cells[picks[i]])in_tail[c]=true;
    
    vector<int> open_cells;
    for(int i=0;i<ne;i++)if(!in_tail[ec[i]])open_cells.push_back(ec[i]);
    if(open_cells.empty())return;
    
    int s0=open_cells[ri(open_cells.size())];
    cs[cm++]={s0,0};
    
    for(int i=0;i<pt;i++){
        int b=picks[i];
        cd[cn++]={br_u[b],br_v[b],i*2+1};
        int sw_cell=tail_cells[b][ri(tail_cells[b].size())];
        cs[cm++]={sw_cell,i+1};
    }
    cd[cn++]={br_u[b_10],br_v[b_10],pt*2+1};
}

void solve(){
    Ti ti;
    if(!(cin>>N>>M>>K))return;
    for(int i=0;i<400;i++){sw[i]=-1;for(int j=0;j<4;j++)dm[i][j]=-1;}
    ne=0;
    for(int i=0;i<N;i++){cin>>G[i];for(int j=0;j<N;j++){int u=i*N+j;if(G[i][j]=='.'){ct[u]=0;if(u!=0&&u!=399)ec[ne++]=u;}else ct[u]=1;}}
    for(int u=0;u<400;u++){
        na[u]=0;if(ct[u]==1)continue;
        int r=u/20,c=u%20,dr[]={-1,1,0,0},dc[]={0,0,-1,1};
        for(int i=0;i<4;i++){int nr=r+dr[i],nc=c+dc[i];if(nr>=0&&nr<N&&nc>=0&&nc<N&&ct[nr*N+nc]==0)aj[u][na[u]++]=nr*N+nc;}
    }

    find_bridges();

    int best_init_sc=-1;
    for(int tries=0;tries<200;tries++){
        bridge_init();
        if(cn>0){
            int sc=bf<false>();
            if(sc>best_init_sc){best_init_sc=sc;sv();}
        }
    }
    
    if(best_init_sc==-1){cn=0;cm=0;sv();}
    
    cn=bn;cm=bm;for(int i=0;i<bn;i++)cd[i]=bd[i];for(int i=0;i<bm;i++)cs[i]=bs[i];
    int gbs=bf<true>();
    
    double t0=10.0,t1=0.1,tl=1.95;
    int it=0,pc=0,stk=0;
    double tp=t0;

    int cur_sc = gbs;

    while(true){
        if((it&31)==0){double el=ti.e();if(el>tl)break;tp=t0*pow(t1/t0,el/tl);}
        it++;
        if(++pc>=250){pc=0;bf<true>();}

        if(stk>=300){
            stk=0;
            cn=bn;for(int i=0;i<bn;i++)cd[i]=bd[i];
            cm=bm;for(int i=0;i<bm;i++)cs[i]=bs[i];
            int nk=max(1,cn/10); // milder kick
            for(int i=0;i<nk;i++){int j=ri(cn);cd[j].g=ri(2*K);}
            nk=max(1,cm/10);
            for(int i=0;i<nk;i++){int j=ri(cm);cs[j].u=ec[ri(ne)];}
            cur_sc=bf<true>();if(cur_sc==-1){cn=0;cm=0;cur_sc=bf<true>();}
            if(cur_sc>gbs){gbs=cur_sc;sv();}
            continue;
        }

        int op=ri(100),ut=-1,ui2;
        D ud;S us;int uv;bool vl=false;
        vector<D> nuke_doors;

        if(op<20&&cn<M&&np>0){
            int ei=ri(np),u=pu[ei],v=pv[ei];bool ex=false;
            for(int i=0;i<cn;i++)if((cd[i].u==u&&cd[i].v==v)||(cd[i].u==v&&cd[i].v==u)){ex=true;break;}
            if(!ex){cd[cn++]={u,v,ri(2*K)};ut=0;vl=true;}
        }
        else if(op<30&&cn<M){
            int u=ec[ri(ne)];if(na[u]>0){int v=aj[u][ri(na[u])];bool ex=false;
            for(int i=0;i<cn;i++)if((cd[i].u==u&&cd[i].v==v)||(cd[i].u==v&&cd[i].v==u)){ex=true;break;}
            if(!ex){cd[cn++]={u,v,ri(2*K)};ut=0;vl=true;}}
        }
        else if(op<40&&cn>0){ui2=ri(cn);ud=cd[ui2];cd[ui2]=cd[--cn];ut=1;vl=true;}
        else if(op<55&&cn>0){ui2=ri(cn);uv=cd[ui2].g;cd[ui2].g=ri(2*K);ut=2;vl=true;}
        else if(op<65){
            int u=ec[ri(ne)],k=ri(K),f=-1;
            for(int i=0;i<cm;i++)if(cs[i].u==u){f=i;break;}
            if(f!=-1){ui2=f;uv=cs[f].t;cs[f].t=k;ut=5;vl=true;}
            else if(cm<50){cs[cm++]={u,k};ut=3;vl=true;}
        }
        else if(op<70&&cm>0){ui2=ri(cm);us=cs[ui2];cs[ui2]=cs[--cm];ut=4;vl=true;}
        else if(op<80&&cm>0){
            ui2=ri(cm);uv=cs[ui2].u;int nu=ec[ri(ne)];bool h=false;
            for(int i=0;i<cm;i++)if(i!=ui2&&cs[i].u==nu){h=true;break;}
            if(!h){cs[ui2].u=nu;ut=6;vl=true;}
        }
        else if(op<90&&cn>0){
            ui2=ri(cn);ud=cd[ui2];int u=ec[ri(ne)];
            if(na[u]>0){int v=aj[u][ri(na[u])];bool ex=false;
            for(int i=0;i<cn;i++){if(i==ui2)continue;if((cd[i].u==u&&cd[i].v==v)||(cd[i].u==v&&cd[i].v==u)){ex=true;break;}}
            if(!ex){cd[ui2].u=u;cd[ui2].v=v;ut=7;vl=true;}}
        }
        else if(op<95&&cm>0){
            ui2=ri(cm); us=cs[ui2]; int tk=cs[ui2].t;
            cs[ui2]=cs[--cm];
            for(int i=cn-1;i>=0;i--){
                if(cd[i].g/2 == tk){
                    nuke_doors.push_back(cd[i]);
                    cd[i]=cd[--cn];
                }
            }
            ut=8;vl=true;
        }
        else if(op<100&&cm>1){
            ui2=ri(cm); int ui3=ri(cm);
            while(ui2==ui3) ui3=ri(cm);
            int tmp=cs[ui2].t; cs[ui2].t=cs[ui3].t; cs[ui3].t=tmp;
            ut=9; vl=true; ud.u=ui3;
        }
        if(!vl)continue;

        int ns=bf<false>();bool ac=false;
        if(ns!=-1){int df=ns-cur_sc;if(df>=0||rf()<exp((double)df/tp))ac=true;}
        if(ac){
            cur_sc=ns; stk=0;
            if(cur_sc>gbs){gbs=cur_sc;sv();}
        }
        else{
            stk++;
            switch(ut){
                case 0:cn--;break;
                case 1:cd[cn]=cd[ui2];cd[ui2]=ud;cn++;break;
                case 2:cd[ui2].g=uv;break;
                case 3:cm--;break;
                case 4:cs[cm]=cs[ui2];cs[ui2]=us;cm++;break;
                case 5:cs[ui2].t=uv;break;
                case 6:cs[ui2].u=uv;break;
                case 7:cd[ui2]=ud;break;
                case 8:
                    cs[cm++]=us; swap(cs[cm-1], cs[ui2]);
                    for(auto d : nuke_doors) cd[cn++]=d;
                    break;
                case 9:
                    swap(cs[ui2].t, cs[ud.u].t);
                    break;
            }
        }
    }

    cout<<bn<<"\n";
    for(int i=0;i<bn;i++){int r1=bd[i].u/20,c1=bd[i].u%20,r2=bd[i].v/20,c2=bd[i].v%20;cout<<((c1==c2)?0:1)<<" "<<min(r1,r2)<<" "<<min(c1,c2)<<" "<<bd[i].g<<"\n";}
    cout<<bm<<"\n";
    for(int i=0;i<bm;i++)cout<<bs[i].u/20<<" "<<bs[i].u%20<<" "<<bs[i].t<<"\n";
}

int main(){ios_base::sync_with_stdio(false);cin.tie(NULL);solve();}