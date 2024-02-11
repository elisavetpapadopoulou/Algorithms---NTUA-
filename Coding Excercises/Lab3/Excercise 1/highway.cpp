#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;


struct Edge {
    ll a;
    ll b;
    ll time;
};

bool max_comp (const Edge& e1, const Edge& e2) {
    return e1.time > e2.time;
}

class DSU { 
    vector<ll> parent; 
    vector <ll> rank; 
  
public: 
    DSU(ll n) 
    { 
        parent.resize(n+1); 
        rank.resize(n+1); 
  
        for (ll i = 0; i < n; i++) { 
            parent[i] = -1; 
            rank[i] = 1; 
        } 
    } 
  
    // Find function 
    ll find(ll i) 
    {
        if (parent[i] == -1){ 
            return i; 
        }
        return parent[i] = find(parent[i]); 
    } 
  
    // Union function 
    void unite(ll x, ll y) 
    { 
        ll s1 = find(x);
        ll s2 = find(y); 
  
        if (s1 != s2) { 
            if (rank[s1] < rank[s2]) { 
                parent[s1] = s2; 
            } 
            else if (rank[s1] > rank[s2]) { 
                parent[s2] = s1; 
            } 
            else { 
                parent[s2] = s1; 
                rank[s1] += 1; 
            } 
        } 
    } 
};

ll kruskal(ll K, ll vertices, ll edges, vector<Edge> edgelist, vector<Edge> &MST){
    DSU s(vertices);
    Edge edge;
    ll MST_size = 0;
    for (ll i=0; i<edges; i++) {
        ll x = edgelist[i].a;       
        ll y = edgelist[i].b;      
        ll t = edgelist[i].time;
        if (s.find(x) != s.find(y)) { 
            s.unite(x, y);
            edge.a = x;
            edge.b = y;
            edge.time = t;
            MST.push_back(edge);
            MST_size++; 
        }
    }
    if (MST_size < K) 
        return 0;
    return MST[MST_size-K-1].time;
}

int main(){
    ll N, M, K;
    vector<Edge> edges;
    vector<Edge> MST;

    cin >> N >> M >> K;
    for (ll i=0; i<M; i++) {
        Edge edge;
        cin >> edge.a >> edge.b >> edge.time;
        edges.push_back(edge);
    }
    sort(edges.begin(), edges.end(), max_comp);
    ll T = kruskal(K, N, M, edges, MST);
    if (T == 0) {
        cout << "infinity" << endl;
    }
    else {
        cout << T << endl;
    }

    return 0;
}
