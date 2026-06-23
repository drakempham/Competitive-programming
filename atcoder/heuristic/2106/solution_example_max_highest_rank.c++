#include <bits/stdc++.h>

using namespace std;

using Clock = chrono::steady_clock;

struct RNG {
    uint64_t x;
    explicit RNG(uint64_t seed=1): x(seed?seed:1) {}
    uint64_t next(){ x^=x<<7; x^=x>>9; return x; }
    int operator()(int n){ return int(next()%uint64_t(n)); }
};

struct DSU {
    vector<int> p,sz;
    explicit DSU(int n=0):p(n),sz(n,1){iota(p.begin(),p.end(),0);}
    int find(int x){while(p[x]!=x){p[x]=p[p[x]];x=p[x];}return x;}
    bool unite(int a,int b){a=find(a);b=find(b);if(a==b)return false;if(sz[a]<sz[b])swap(a,b);p[b]=a;sz[a]+=sz[b];return true;}
};

struct Edge { int u,v,d,i,j; };

struct Candidate {
    vector<int> door;
    vector<int> sw;
    vector<int> region;
    vector<int> s0Options;
    vector<int> mutableClosed, mutableOpen;
    long long heuristic=LLONG_MIN;
    int crossLeft=0;
    int kind=0;
    int exactT=-1;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N,M,K;
    if(!(cin>>N>>M>>K)) return 0;
    vector<string> grid(N);
    for(auto &s:grid) cin>>s;

    vector<vector<int>> id(N,vector<int>(N,-1));
    vector<pair<int,int>> pos;
    for(int i=0;i<N;i++)for(int j=0;j<N;j++)if(grid[i][j]=='.'){
        id[i][j]=int(pos.size()); pos.push_back({i,j});
    }
    int V=pos.size();
    int S=id[0][0], T=id[N-1][N-1];

    vector<Edge> edges;
    vector<vector<pair<int,int>>> adj(V);
    auto addEdge=[&](int u,int v,int d,int i,int j){
        int e=edges.size(); edges.push_back({u,v,d,i,j});
        adj[u].push_back({v,e}); adj[v].push_back({u,e});
    };
    for(int i=0;i<N;i++)for(int j=0;j<N;j++)if(id[i][j]>=0){
        if(i+1<N && id[i+1][j]>=0) addEdge(id[i][j],id[i+1][j],0,i,j);
        if(j+1<N && id[i][j+1]>=0) addEdge(id[i][j],id[i][j+1],1,i,j);
    }
    int E=edges.size();

    vector<char> active(V,0);
    queue<int> aq; active[S]=1; aq.push(S);
    while(!aq.empty()){
        int u=aq.front();aq.pop();
        for(auto [v,e]:adj[u]) if(v!=T && !active[v]){active[v]=1;aq.push(v);}
    }
    vector<int> activeVerts;
    for(int v=0;v<V;v++)if(active[v])activeVerts.push_back(v);
    vector<int> goalEdges;
    for(auto [v,e]:adj[T])goalEdges.push_back(e);

    uint64_t seed=0xcbf29ce484222325ULL;
    for(auto &s:grid)for(unsigned char ch:s){seed^=ch;seed*=0x100000001b3ULL;}
    seed^=uint64_t(V)*0x9e3779b97f4a7c15ULL;
    RNG rng(seed);

    auto startTime=Clock::now();
    auto elapsed=[&](){return chrono::duration<double>(Clock::now()-startTime).count();};

    vector<int> seen(V*(1<<K),0), distState(V*(1<<K)), qstate(V*(1<<K));
    int stamp=0;
    auto calcT=[&](const Candidate &cand)->int{
        ++stamp;
        int head=0,tail=0;
        int st=S;
        seen[st]=stamp;distState[st]=0;qstate[tail++]=st;
        while(head<tail){
            int z=qstate[head++],mask=z/V,u=z-mask*V,d=distState[z];
            if(u==T)return d;
            for(auto [v,e]:adj[u]){
                int g=cand.door[e];
                if(g>=0 && (((mask>>(g>>1))&1)!=(g&1)))continue;
                int nz=mask*V+v;
                if(seen[nz]!=stamp){seen[nz]=stamp;distState[nz]=d+1;qstate[tail++]=nz;}
            }
            int k=cand.sw[u];
            if(k>=0){
                int nm=mask^(1<<k),nz=nm*V+u;
                if(seen[nz]!=stamp){seen[nz]=stamp;distState[nz]=d+1;qstate[tail++]=nz;}
            }
        }
        return 0;
    };

    auto logicalType=[&](int a,int b)->int{
        if(a>b)swap(a,b);
        for(int i=0;i<9;i++){
            if(a==i && b==i+1)return 2*i;
            if(a==i && b==10+i)return 2*i+1;
        }
        if(a==9 && b==19)return 19;
        return -1;
    };

    auto abstractT=[&](const vector<int>& types,const vector<pair<int,int>>& pairs)->int{
        static vector<vector<pair<int,int>>> G(20);
        for(auto &x:G)x.clear();
        for(int i=0;i<9;i++){
            G[i].push_back({i+1,2*i});G[i+1].push_back({i,2*i});
            G[i].push_back({10+i,2*i+1});G[10+i].push_back({i,2*i+1});
        }
        G[9].push_back({19,19});G[19].push_back({9,19});
        for(int z=0;z<(int)pairs.size();z++){
            auto [a,b]=pairs[z];int g=types[z];
            G[a].push_back({b,g});G[b].push_back({a,g});
        }
        static int dd[1<<10][20];
        static int qq[(1<<10)*20];
        for(int m=0;m<(1<<10);m++)for(int r=0;r<20;r++)dd[m][r]=-1;
        int h=0,t=0;dd[0][0]=0;qq[t++]=0;
        while(h<t){
            int z=qq[h++],m=z/20,u=z%20,d=dd[m][u];
            if(u==19)return d;
            for(auto [v,g]:G[u]){
                if(((m>>(g>>1))&1)!=(g&1))continue;
                if(dd[m][v]<0){dd[m][v]=d+1;qq[t++]=m*20+v;}
            }
            int k=-1;if(u==0)k=0;else if(10<=u&&u<=18)k=u-9;
            if(k>=0){int nm=m^(1<<k);if(dd[nm][u]<0){dd[nm][u]=d+1;qq[t++]=nm*20+u;}}
        }
        return 0;
    };

    vector<Candidate> fullPool, activePool;
    const int FULL_KEEP=80, ACTIVE_KEEP=56;
    auto insertFull=[&](Candidate &&c){
        fullPool.push_back(std::move(c));
        if((int)fullPool.size()>FULL_KEEP*2){
            nth_element(fullPool.begin(),fullPool.begin()+FULL_KEEP,fullPool.end(),[](const Candidate&a,const Candidate&b){return a.heuristic>b.heuristic;});
            fullPool.resize(FULL_KEEP);
        }
    };
    auto insertActive=[&](Candidate &&c){
        activePool.push_back(std::move(c));
        if((int)activePool.size()>ACTIVE_KEEP*2){
            nth_element(activePool.begin(),activePool.begin()+ACTIVE_KEEP,activePool.end(),[](const Candidate&a,const Candidate&b){
                if(a.crossLeft!=b.crossLeft)return a.crossLeft<b.crossLeft;
                return a.heuristic>b.heuristic;
            });
            activePool.resize(ACTIVE_KEEP);
        }
    };

    vector<int> parent(V),pedge(V),depth(V),order;
    vector<vector<int>> children(V);
    vector<char> isTree(E);

    auto treeDistance=[&](int a,int b)->int{
        int x=a,y=b,d=0;
        while(depth[x]>depth[y]){x=parent[x];d++;}
        while(depth[y]>depth[x]){y=parent[y];d++;}
        while(x!=y){x=parent[x];y=parent[y];d+=2;}
        return d;
    };

    int attempts=0;
    while(elapsed()<1.16){
        attempts++;
        fill(parent.begin(),parent.end(),-1);
        fill(pedge.begin(),pedge.end(),-1);
        fill(depth.begin(),depth.end(),0);
        fill(isTree.begin(),isTree.end(),0);
        for(auto &v:children)v.clear();
        order.clear();order.reserve(V);

        vector<vector<pair<int,int>>> radj=adj;
        int mode=attempts%7;
        for(int u=0;u<V;u++){
            for(int i=(int)radj[u].size()-1;i>0;i--)swap(radj[u][i],radj[u][rng(i+1)]);
            if(mode==1 || mode==2){
                stable_sort(radj[u].begin(),radj[u].end(),[&](auto a,auto b){
                    int da=adj[a.first].size(),db=adj[b.first].size();
                    return mode==1?da<db:da>db;
                });
            }else if(mode==3 || mode==4){
                stable_sort(radj[u].begin(),radj[u].end(),[&](auto a,auto b){
                    auto [ai,aj]=pos[a.first];auto [bi,bj]=pos[b.first];
                    int xa=abs(ai-(N-1))+abs(aj-(N-1));
                    int xb=abs(bi-(N-1))+abs(bj-(N-1));
                    return mode==3?xa>xb:xa<xb;
                });
            }
        }

        auto runDFS=[&](bool skipGoal){
            fill(parent.begin(),parent.end(),-1);
            fill(pedge.begin(),pedge.end(),-1);
            fill(depth.begin(),depth.end(),0);
            fill(isTree.begin(),isTree.end(),0);
            for(auto &v:children)v.clear();
            order.clear();
            vector<int> it(V,0),st;st.reserve(V);
            parent[S]=S;st.push_back(S);order.push_back(S);
            while(!st.empty()){
                int u=st.back();
                if(it[u]==(int)radj[u].size()){st.pop_back();continue;}
                auto [v,e]=radj[u][it[u]++];
                if(skipGoal && v==T)continue;
                if(parent[v]>=0)continue;
                parent[v]=u;pedge[v]=e;depth[v]=depth[u]+1;isTree[e]=1;children[u].push_back(v);order.push_back(v);st.push_back(v);
            }
        };

        bool delayed=(attempts%5)!=0;
        runDFS(delayed);
        if(delayed){
            bool all=true;
            for(int v=0;v<V;v++)if(v!=T && parent[v]<0)all=false;
            int bv=-1,be=-1;
            if(all){
                for(auto [v,e]:adj[T])if(parent[v]>=0 && (bv<0 || depth[v]>depth[bv])){bv=v;be=e;}
            }
            if(bv>=0){parent[T]=bv;pedge[T]=be;depth[T]=depth[bv]+1;isTree[be]=1;children[bv].push_back(T);order.push_back(T);}
            else runDFS(false);
        }
        if(parent[T]<0)continue;

        vector<int> deep(V),subsz(V,1);
        for(int v=0;v<V;v++)deep[v]=v;
        for(int z=(int)order.size()-1;z>0;z--){
            int v=order[z],p=parent[v];
            subsz[p]+=subsz[v];
            if(depth[deep[v]]>depth[deep[p]])deep[p]=deep[v];
        }

        {
            vector<int> path;
            for(int u=T;;u=parent[u]){path.push_back(u);if(u==S)break;}
            reverse(path.begin(),path.end());
            int L=(int)path.size()-1;
            if(L>=11){
                vector<int> pathIndex(V,-1);
                for(int i=0;i<=L;i++)pathIndex[path[i]]=i;
                struct Branch{int child=-1,far=-1,dep=0,sz=0;};
                vector<Branch> br(L+1);
                for(int j=0;j<L;j++){
                    int mainChild=path[j+1];
                    for(int ch:children[path[j]])if(ch!=mainChild){
                        int f=deep[ch],dep=depth[f]-depth[path[j]];
                        if(br[j].child<0 || make_pair(dep,subsz[ch])>make_pair(br[j].dep,br[j].sz))br[j]={ch,f,dep,subsz[ch]};
                    }
                }
                const long long NEG=-(1LL<<60);
                vector<vector<long long>> dp(9,vector<long long>(L+1,NEG));
                vector<vector<int>> pre(9,vector<int>(L+1,-1));
                for(int j=0;j<=L-2;j++)if(br[j].child>=0){
                    long long x=j+br[j].dep;
                    dp[0][j]=256LL*x*4096 + 32LL*br[j].sz + rng(1024);
                }
                for(int lev=1;lev<9;lev++){
                    long long best=NEG;int bi=-1,w=1<<(8-lev);
                    for(int j=0;j<=L-2;j++){
                        if(j>0 && dp[lev-1][j-1]>best){best=dp[lev-1][j-1];bi=j-1;}
                        if(best==NEG || br[j].child<0)continue;
                        long long x=j+br[j].dep;
                        dp[lev][j]=best+1LL*w*x*4096+32LL*br[j].sz+rng(1024);
                        pre[lev][j]=bi;
                    }
                }
                int bj=-1;
                for(int j=0;j<=L-2;j++)if(bj<0||dp[8][j]>dp[8][bj])bj=j;
                if(bj>=0 && dp[8][bj]>NEG/2){
                    vector<int> choose(9);
                    int cur=bj;for(int lev=8;lev>=0;lev--){choose[lev]=cur;cur=pre[lev][cur];}
                    vector<char> cut(E,0),gate(E,0);
                    vector<int> branchChild(9),far(9);
                    bool ok=true;
                    for(int lev=0;lev<9;lev++){
                        int j=choose[lev];
                        int ce=pedge[path[j+1]],be=pedge[br[j].child];
                        if(ce<0||be<0||ce==be){ok=false;break;}
                        cut[ce]=cut[be]=gate[ce]=gate[be]=1;
                        branchChild[lev]=br[j].child;far[lev]=br[j].far;
                    }
                    int finalEdge=pedge[T];
                    if(finalEdge<0||gate[finalEdge])ok=false;else cut[finalEdge]=gate[finalEdge]=1;
                    if(ok){
                        DSU dsu(V);
                        for(int v=0;v<V;v++)if(v!=S){int e=pedge[v];if(!cut[e])dsu.unite(v,parent[v]);}
                        vector<int> roots;
                        roots.push_back(dsu.find(S));
                        for(int lev=0;lev<9;lev++){roots.push_back(dsu.find(path[choose[lev]+1]));roots.push_back(dsu.find(branchChild[lev]));}
                        roots.push_back(dsu.find(T));
                        sort(roots.begin(),roots.end());roots.erase(unique(roots.begin(),roots.end()),roots.end());
                        if((int)roots.size()==20){
                            unordered_map<int,int> label;
                            label[dsu.find(S)]=0;
                            for(int lev=0;lev<9;lev++){
                                label[dsu.find(path[choose[lev]+1])]=lev+1;
                                label[dsu.find(branchChild[lev])]=10+lev;
                            }
                            label[dsu.find(T)]=19;
                            vector<int> region(V,-1);
                            bool labelsOK=true;
                            for(int v=0;v<V;v++){
                                auto it=label.find(dsu.find(v));
                                if(it==label.end()){labelsOK=false;break;}
                                region[v]=it->second;
                            }
                            if(labelsOK){
                                vector<int> door(E,-1);
                                map<pair<int,int>,vector<int>> extras;
                                int D=0;
                                for(int e=0;e<E;e++){
                                    int a=region[edges[e].u],b=region[edges[e].v];
                                    if(a==b)continue;
                                    D++;
                                    int ty=logicalType(a,b);
                                    if(ty>=0)door[e]=ty;
                                    else{if(a>b)swap(a,b);extras[{a,b}].push_back(e);}
                                }
                                if(D<=M){
                                    vector<pair<int,int>> pairs;
                                    vector<int> ptypes;
                                    for(auto &kv:extras){
                                        pairs.push_back(kv.first);
                                        int a=kv.first.first,b=kv.first.second;
                                        auto levelOf=[](int r){return r<10?r:(r==19?10:r-10);};
                                        int dep=min(levelOf(a),levelOf(b));
                                        int ty=dep>0?2*(dep-1)+1:19;
                                        ptypes.push_back(ty);
                                        for(int e:kv.second)door[e]=ty;
                                    }

                                    vector<int> sw(V,-1);
                                    for(int lev=0;lev<9;lev++)sw[far[lev]]=lev+1;
                                    static const int coeff[9]={512,256,128,64,32,16,8,4,2};
                                    vector<pair<long long,int>> opts;
                                    for(int v=0;v<V;v++)if(region[v]==0){
                                        long long val=treeDistance(S,v)+treeDistance(v,T);
                                        for(int lev=0;lev<9;lev++)val+=1LL*coeff[lev]*treeDistance(v,far[lev]);
                                        opts.push_back({val,v});
                                    }
                                    sort(opts.begin(),opts.end(),greater<pair<long long,int>>());
                                    if(!opts.empty()){
                                        int s0=opts[0].second;sw[s0]=0;
                                        struct Add{long long pri;int e;};vector<Add> add;
                                        for(int e=0;e<E;e++)if(!isTree[e]&&door[e]<0){
                                            int u=edges[e].u,v=edges[e].v;
                                            if(region[u]!=region[v])continue;
                                            long long save=0;
                                            for(int lev=0;lev<9;lev++){
                                                int base=treeDistance(s0,far[lev]);
                                                int alt=min(treeDistance(s0,u)+1+treeDistance(v,far[lev]),treeDistance(s0,v)+1+treeDistance(u,far[lev]));
                                                if(alt<base)save+=1LL*coeff[lev]*(base-alt);
                                            }
                                            long long pri=save*10000LL+1LL*treeDistance(u,v)*treeDistance(u,v);
                                            add.push_back({pri,e});
                                        }
                                        sort(add.begin(),add.end(),[](const Add&a,const Add&b){return a.pri>b.pri;});
                                        int phaseType=17;
                                        for(auto x:add)if(D<M){door[x.e]=phaseType;D++;}

                                        int abs=abstractT(ptypes,pairs);
                                        Candidate cand;
                                        cand.door=std::move(door);cand.sw=std::move(sw);cand.region=std::move(region);cand.kind=0;
                                        for(int z=0;z<min<int>(5,opts.size());z++)cand.s0Options.push_back(opts[z].second);
                                        cand.heuristic=opts[0].first*1000LL - 20000LL*(int)pairs.size() - 200LL*D + 2000LL*abs;
                                        insertFull(std::move(cand));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        {
            vector<int> activeDeep(V),activeSize(V,0);
            for(int v=0;v<V;v++){activeDeep[v]=v;if(active[v])activeSize[v]=1;}
            for(int z=(int)order.size()-1;z>0;z--){
                int v=order[z];if(!active[v])continue;int p=parent[v];
                if(p>=0&&active[p]){
                    activeSize[p]+=activeSize[v];
                    if(depth[activeDeep[v]]>depth[activeDeep[p]])activeDeep[p]=activeDeep[v];
                }
            }
            long long bestObj=LLONG_MIN;
            vector<int> bestPath,bestChoose,bestBranch,bestFar;
            for(int endpoint:activeVerts){
                if(parent[endpoint]<0)continue;
                vector<int> path;
                for(int u=endpoint;;u=parent[u]){path.push_back(u);if(u==S)break;}
                reverse(path.begin(),path.end());
                int L=path.size();if(L<10)continue;
                struct B{int child=-1,far=-1,dep=0,sz=0;};vector<B> br(L);
                for(int j=0;j+1<L;j++){
                    int mainChild=path[j+1];
                    for(int ch:children[path[j]])if(ch!=mainChild&&active[ch]){
                        int f=activeDeep[ch],dep=depth[f]-depth[path[j]];
                        if(br[j].child<0||make_pair(dep,activeSize[ch])>make_pair(br[j].dep,br[j].sz))br[j]={ch,f,dep,activeSize[ch]};
                    }
                }
                const long long NEG=-(1LL<<60);
                vector<vector<long long>> dp(9,vector<long long>(L,NEG));
                vector<vector<int>> pre(9,vector<int>(L,-1));
                static const int w[9]={512,256,128,64,32,16,8,4,1};
                for(int j=0;j+1<L;j++)if(br[j].child>=0)dp[0][j]=1LL*w[0]*(j+br[j].dep)*1024+32LL*br[j].sz+rng(256);
                for(int lev=1;lev<9;lev++){
                    long long pref=NEG;int pi=-1;
                    for(int j=0;j+1<L;j++){
                        if(j>0&&dp[lev-1][j-1]>pref){pref=dp[lev-1][j-1];pi=j-1;}
                        if(pref==NEG||br[j].child<0)continue;
                        dp[lev][j]=pref+1LL*w[lev]*(j+br[j].dep)*1024+32LL*br[j].sz+rng(256);pre[lev][j]=pi;
                    }
                }
                int bj=-1;for(int j=0;j+1<L;j++)if(bj<0||dp[8][j]>dp[8][bj])bj=j;
                if(bj<0||dp[8][bj]<=NEG/2)continue;
                long long obj=dp[8][bj];
                if(obj<=bestObj)continue;
                vector<int> choose(9),bc(9),bf(9);int cur=bj;
                for(int lev=8;lev>=0;lev--){choose[lev]=cur;bc[lev]=br[cur].child;bf[lev]=br[cur].far;cur=pre[lev][cur];}
                bestObj=obj;bestPath=std::move(path);bestChoose=std::move(choose);bestBranch=std::move(bc);bestFar=std::move(bf);
            }
            if(bestObj>LLONG_MIN/2){
                vector<char> gate(E,0);
                vector<int> door(E,-1);
                bool ok=true;int D=0;
                for(int lev=0;lev<9;lev++){
                    int ce=pedge[bestPath[bestChoose[lev]+1]],be=pedge[bestBranch[lev]];
                    if(ce<0||be<0||ce==be){ok=false;break;}
                    gate[ce]=gate[be]=1;door[ce]=2*lev;door[be]=2*lev+1;D+=2;
                }
                for(int e:goalEdges){if(door[e]<0){door[e]=19;D++;}}
                if(ok&&D<=M){
                    DSU dsu(V);
                    for(int v:activeVerts)if(v!=S){int e=pedge[v];if(!gate[e])dsu.unite(v,parent[v]);}
                    int root=dsu.find(S);
                    unordered_map<int,int> activeLabel;
                    activeLabel[root]=0;
                    bool regionOK=true;
                    for(int lev=0;lev<9;lev++){
                        int ra=dsu.find(bestPath[bestChoose[lev]+1]);
                        int rb=dsu.find(bestBranch[lev]);
                        if((activeLabel.count(ra)&&activeLabel[ra]!=lev+1) || (activeLabel.count(rb)&&activeLabel[rb]!=10+lev))regionOK=false;
                        activeLabel[ra]=lev+1;activeLabel[rb]=10+lev;
                    }
                    vector<int> activeRegion(V,-1);activeRegion[T]=19;
                    for(int v:activeVerts){auto it=activeLabel.find(dsu.find(v));if(it==activeLabel.end())regionOK=false;else activeRegion[v]=it->second;}
                    static const int coeff[9]={512,256,128,64,32,16,8,4,1};
                    vector<pair<long long,int>> opts;
                    for(int v:activeVerts)if(dsu.find(v)==root){
                        long long val=2LL*treeDistance(S,v);
                        for(int lev=0;lev<9;lev++)val+=1LL*coeff[lev]*treeDistance(v,bestFar[lev]);
                        opts.push_back({val,v});
                    }
                    sort(opts.begin(),opts.end(),greater<pair<long long,int>>());
                    if(regionOK && !opts.empty()){
                        int s0=opts[0].second;
                        vector<int> sw(V,-1);sw[s0]=0;for(int lev=0;lev<9;lev++)sw[bestFar[lev]]=lev+1;
                        struct NE{int e,cross;long long benefit;int td;};vector<NE> ne;
                        int crossTotal=0;
                        for(int e=0;e<E;e++){
                            int u=edges[e].u,v=edges[e].v;
                            if(!active[u]||!active[v]||isTree[e])continue;
                            int cross=dsu.find(u)!=dsu.find(v);crossTotal+=cross;
                            long long benefit=0;
                            for(int lev=0;lev<9;lev++){
                                int base=treeDistance(s0,bestFar[lev]);
                                int alt=min(treeDistance(s0,u)+1+treeDistance(v,bestFar[lev]),treeDistance(s0,v)+1+treeDistance(u,bestFar[lev]));
                                if(alt<base)benefit+=1LL*coeff[lev]*(base-alt);
                            }
                            ne.push_back({e,cross,benefit,treeDistance(u,v)});
                        }
                        sort(ne.begin(),ne.end(),[](const NE&a,const NE&b){
                            if(a.cross!=b.cross)return a.cross>b.cross;
                            if(a.benefit!=b.benefit)return a.benefit>b.benefit;
                            return a.td>b.td;
                        });
                        int closedCross=0;
                        vector<int> mutableClosed,mutableOpen;
                        for(auto x:ne){
                            if(D<M){door[x.e]=19;D++;closedCross+=x.cross;mutableClosed.push_back(x.e);}
                            else mutableOpen.push_back(x.e);
                        }
                        Candidate cand;cand.door=std::move(door);cand.sw=std::move(sw);cand.region=std::move(activeRegion);cand.kind=1;cand.crossLeft=crossTotal-closedCross;
                        cand.mutableClosed=std::move(mutableClosed);cand.mutableOpen=std::move(mutableOpen);
                        for(int z=0;z<min<int>(5,opts.size());z++)cand.s0Options.push_back(opts[z].second);
                        cand.heuristic=opts[0].first-1000000000000LL*cand.crossLeft;
                        insertActive(std::move(cand));
                    }
                }
            }
        }
    }

    sort(fullPool.begin(),fullPool.end(),[](const Candidate&a,const Candidate&b){return a.heuristic>b.heuristic;});
    if((int)fullPool.size()>FULL_KEEP)fullPool.resize(FULL_KEEP);
    sort(activePool.begin(),activePool.end(),[](const Candidate&a,const Candidate&b){if(a.crossLeft!=b.crossLeft)return a.crossLeft<b.crossLeft;return a.heuristic>b.heuristic;});
    if((int)activePool.size()>ACTIVE_KEEP)activePool.resize(ACTIVE_KEEP);

    for(int ci=0;ci<min<int>(10,fullPool.size()) && elapsed()<1.58;ci++){
        Candidate &cand=fullPool[ci];
        map<pair<int,int>,vector<int>> mp;
        for(int e=0;e<E;e++){
            int a=cand.region[edges[e].u],b=cand.region[edges[e].v];
            if(a==b||logicalType(a,b)>=0)continue;
            if(a>b)swap(a,b);mp[{a,b}].push_back(e);
        }
        vector<pair<int,int>> pairs;vector<int> ty;
        for(auto &kv:mp){pairs.push_back(kv.first);ty.push_back(cand.door[kv.second[0]]);}
        int best=abstractT(ty,pairs);
        for(int pass=0;pass<2&&elapsed()<1.58;pass++){
            vector<int> ord(pairs.size());iota(ord.begin(),ord.end(),0);
            for(int i=(int)ord.size()-1;i>0;i--)swap(ord[i],ord[rng(i+1)]);
            for(int z:ord){
                int old=ty[z],bt=old,bv=best;
                for(int g=0;g<20;g++){
                    ty[z]=g;int x=abstractT(ty,pairs);
                    if(x>bv){bv=x;bt=g;}
                }
                ty[z]=bt;best=bv;
                if(elapsed()>=1.58)break;
            }
        }
        for(int z=0;z<(int)pairs.size();z++)for(int e:mp[pairs[z]])cand.door[e]=ty[z];
    }

    Candidate answer, bestFullAnswer, bestActiveAnswer;
    int bestT=-1, bestFullT=-1, bestActiveT=-1;
    auto tryCandidate=[&](Candidate cand,int optionLimit){
        vector<int> opts=cand.s0Options;
        if(opts.empty())opts.push_back(-1);
        int old0=-1;for(int v=0;v<V;v++)if(cand.sw[v]==0){old0=v;break;}
        int lim=min<int>(optionLimit,opts.size());
        for(int z=0;z<lim;z++){
            if(old0>=0)cand.sw[old0]=-1;
            int nv=opts[z];if(nv<0)nv=old0;
            if(nv>=0)cand.sw[nv]=0;
            int t=calcT(cand);
            if(t>bestT){bestT=t;answer=cand;answer.exactT=t;}
            if(cand.kind==0 && t>bestFullT){bestFullT=t;bestFullAnswer=cand;bestFullAnswer.exactT=t;}
            if(cand.kind==1 && t>bestActiveT){bestActiveT=t;bestActiveAnswer=cand;bestActiveAnswer.exactT=t;}
            if(nv>=0)cand.sw[nv]=-1;
            old0=nv;
        }
    };

    int fi=0,ai=0;
    while((fi<(int)fullPool.size()||ai<(int)activePool.size()) && elapsed()<1.90){
        if(fi<(int)fullPool.size()){tryCandidate(fullPool[fi],fi<5?3:1);fi++;}
        if(ai<(int)activePool.size()){tryCandidate(activePool[ai],ai<5?3:1);ai++;}
    }
    if(bestT<0){
        if(!fullPool.empty())tryCandidate(fullPool[0],1);
        if(!activePool.empty())tryCandidate(activePool[0],1);
    }

    if(bestFullT>0 && elapsed()<1.90){
        Candidate cur=bestFullAnswer;
        int curT=bestFullT;

        map<pair<int,int>,vector<int>> extraPairs;
        for(int e=0;e<E;e++){
            int a=cur.region[edges[e].u],b=cur.region[edges[e].v];
            if(a==b || logicalType(a,b)>=0)continue;
            if(a>b)swap(a,b);
            extraPairs[{a,b}].push_back(e);
        }
        vector<pair<int,int>> pairOrder;
        for(auto &kv:extraPairs)pairOrder.push_back(kv.first);
        for(int i=(int)pairOrder.size()-1;i>0;i--)swap(pairOrder[i],pairOrder[rng(i+1)]);
        const int typeOrder[20]={17,19,15,13,11,9,7,5,3,1,16,18,14,12,10,8,6,4,2,0};
        for(auto key:pairOrder){
            if(elapsed()>=1.90)break;
            auto &es=extraPairs[key];
            int old=cur.door[es[0]],bestType=old,localBest=curT;
            for(int z=0;z<20 && elapsed()<1.90;z++){
                int g=typeOrder[z];
                if(g==old)continue;
                for(int e:es)cur.door[e]=g;
                int t=calcT(cur);
                if(t>localBest){localBest=t;bestType=g;}
            }
            for(int e:es)cur.door[e]=bestType;
            curT=localBest;
        }

        vector<int> internal;
        for(int e=0;e<E;e++)if(cur.door[e]>=0 && cur.region[edges[e].u]==cur.region[edges[e].v])internal.push_back(e);
        if(!internal.empty()){
            vector<int> saved;for(int e:internal)saved.push_back(cur.door[e]);
            int bestType=-1,localBest=curT;
            for(int g:{17,15,19})if(elapsed()<1.90){
                for(int e:internal)cur.door[e]=g;
                int t=calcT(cur);if(t>localBest){localBest=t;bestType=g;}
            }
            if(bestType>=0)for(int e:internal)cur.door[e]=bestType;
            else for(int z=0;z<(int)internal.size();z++)cur.door[internal[z]]=saved[z];
            curT=localBest;
        }

        for(int k=1;k<10 && elapsed()<1.90;k++){
            int reg=9+k;
            vector<int> sources;
            for(int e=0;e<E;e++){
                int u=edges[e].u,v=edges[e].v;
                if(cur.region[u]==reg && cur.region[v]==k-1)sources.push_back(u);
                if(cur.region[v]==reg && cur.region[u]==k-1)sources.push_back(v);
            }
            sort(sources.begin(),sources.end());sources.erase(unique(sources.begin(),sources.end()),sources.end());
            if(sources.empty())continue;
            vector<int> dd(V,-1),qq;qq.reserve(V);
            for(int v:sources){dd[v]=0;qq.push_back(v);}
            for(int qi=0;qi<(int)qq.size();qi++){
                int u=qq[qi];
                for(auto [v,e]:adj[u])if(cur.region[v]==reg && dd[v]<0){dd[v]=dd[u]+1;qq.push_back(v);}
            }
            vector<pair<int,int>> opts;
            for(int v=0;v<V;v++)if(cur.region[v]==reg)opts.push_back({dd[v],v});
            sort(opts.begin(),opts.end(),greater<pair<int,int>>());
            int old=-1;for(int v=0;v<V;v++)if(cur.sw[v]==k){old=v;break;}
            int bestV=old,localBest=curT;
            if(old>=0)cur.sw[old]=-1;
            for(int z=0;z<min<int>(4,opts.size()) && elapsed()<1.90;z++){
                int nv=opts[z].second;if(cur.sw[nv]>=0)continue;
                cur.sw[nv]=k;int t=calcT(cur);cur.sw[nv]=-1;
                if(t>localBest){localBest=t;bestV=nv;}
            }
            if(bestV>=0)cur.sw[bestV]=k;
            curT=localBest;
        }

        if(curT>bestFullT){bestFullT=curT;bestFullAnswer=cur;bestFullAnswer.exactT=curT;}
        if(curT>bestT){bestT=curT;answer=cur;answer.exactT=curT;}
    }

    if(bestActiveT>0 && elapsed()<1.94){
        Candidate cur=bestActiveAnswer;
        int curT=bestActiveT;

        for(int k=1;k<10 && elapsed()<1.94;k++){
            int reg=9+k;
            vector<int> sources;
            for(int e=0;e<E;e++){
                int u=edges[e].u,v=edges[e].v;
                if(cur.region[u]==reg && cur.region[v]==k-1)sources.push_back(u);
                if(cur.region[v]==reg && cur.region[u]==k-1)sources.push_back(v);
            }
            sort(sources.begin(),sources.end());sources.erase(unique(sources.begin(),sources.end()),sources.end());
            if(sources.empty())continue;
            vector<int> dd(V,-1),qq;qq.reserve(V);
            for(int v:sources){dd[v]=0;qq.push_back(v);}
            for(int qi=0;qi<(int)qq.size();qi++){
                int u=qq[qi];
                for(auto [v,e]:adj[u])if(cur.region[v]==reg && dd[v]<0){dd[v]=dd[u]+1;qq.push_back(v);}
            }
            vector<pair<int,int>> opts;
            for(int v=0;v<V;v++)if(cur.region[v]==reg)opts.push_back({dd[v],v});
            sort(opts.begin(),opts.end(),greater<pair<int,int>>());
            int old=-1;for(int v=0;v<V;v++)if(cur.sw[v]==k){old=v;break;}
            int bestV=old,localBest=curT;
            if(old>=0)cur.sw[old]=-1;
            for(int z=0;z<min<int>(4,opts.size()) && elapsed()<1.94;z++){
                int nv=opts[z].second;if(cur.sw[nv]>=0)continue;
                cur.sw[nv]=k;int t=calcT(cur);cur.sw[nv]=-1;
                if(t>localBest){localBest=t;bestV=nv;}
            }
            if(bestV>=0)cur.sw[bestV]=k;
            curT=localBest;
        }

        int trials=0;
        while(!cur.mutableClosed.empty() && !cur.mutableOpen.empty() && elapsed()<1.94 && trials<60){
            int a=rng(cur.mutableClosed.size()),b=rng(cur.mutableOpen.size());
            int ce=cur.mutableClosed[a],oe=cur.mutableOpen[b];
            cur.door[ce]=-1;cur.door[oe]=19;
            int t=calcT(cur);
            if(t>curT){
                curT=t;swap(cur.mutableClosed[a],cur.mutableOpen[b]);
            }else{
                cur.door[ce]=19;cur.door[oe]=-1;
            }
            trials++;
        }

        if(curT>bestActiveT){bestActiveT=curT;bestActiveAnswer=cur;bestActiveAnswer.exactT=curT;}
        if(curT>bestT){bestT=curT;answer=cur;answer.exactT=curT;}
    }

    if(bestT<=0){
        cout<<0<<'\n'<<0<<'\n';
        return 0;
    }

    vector<int> outE;
    for(int e=0;e<E;e++)if(answer.door[e]>=0)outE.push_back(e);
    cout<<outE.size()<<'\n';
    for(int e:outE){
        const auto &x=edges[e];
        cout<<x.d<<' '<<x.i<<' '<<x.j<<' '<<answer.door[e]<<'\n';
    }
    vector<int> outS;
    for(int v=0;v<V;v++)if(answer.sw[v]>=0)outS.push_back(v);
    cout<<outS.size()<<'\n';
    for(int v:outS)cout<<pos[v].first<<' '<<pos[v].second<<' '<<answer.sw[v]<<'\n';
    return 0;
}
