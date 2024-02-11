#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
int gcd(int a, int b) 
{ 
    int result = min(a, b); 
    while (result > 0) { 
        if (a % result == 0 && b % result == 0) { 
            break; 
        } 
        result--; 
    } 
  
    return result; 
}


bool binary_weight_comp (const std::vector<double>& e1, const std::vector<double>& e2) {
    return e1[0] > e2[0];
}

bool profit_comp (const std::vector<double>& e1, const std::vector<double>& e2) {
    return e1[3] > e2[3];
}

bool weight_comp (const std::vector<double>& e1, const std::vector<double>& e2) {
    return e1[4] < e2[4];
}

struct Kruskal_Result {
    double profit_sum = 0;
    double weight_sum = 0;
    int gcd_of_sums;
};

// DSU data structure 
// path compression + rank by union 

class DSU { 
    int* parent; 
    int* rank; 
  
public: 
    DSU(int n) 
    { 
        parent = new int[n]; 
        rank = new int[n]; 
  
        for (int i = 0; i < n; i++) { 
            parent[i] = -1; 
            rank[i] = 1; 
        } 
    } 
  
    // Find function 
    int find(int i) 
    { 
        if (parent[i] == -1) 
            return i; 
        
        return parent[i] = find(parent[i]); 
    } 
  
    // Union function 
    void unite(int x, int y) 
    { 
        int s1 = find(x); 
        int s2 = find(y); 
  
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

struct Kruskal_Result kruskal(int vertices, int edges, vector<vector<double>> edgelist){
    DSU s(vertices);
    struct Kruskal_Result ans;
    for (int i=0; i<edges; i++) {
        double bw = edgelist[i][0];
        int x = edgelist[i][1];
        int y = edgelist[i][2];
        
        if (s.find(x) != s.find(y)) { 
            s.unite(x, y); 
            ans.profit_sum += edgelist[i][3]; 
            ans.weight_sum += edgelist[i][4];
        }
    } 
    ans.gcd_of_sums = gcd(ans.profit_sum, ans.weight_sum);
    return ans;
}

void MaximumRatioTree (int vertices, int edges, vector<vector<double>> edgelist){
    double c, upper_bound, lower_bound, threshold; 
    threshold = 0.001;
    struct Kruskal_Result result;

    vector<vector<double> > edgelist1 = edgelist;
    vector<vector<double> > edgelist2 = edgelist;

    sort(edgelist1.begin(), edgelist1.end(), profit_comp);
    struct Kruskal_Result maximum_spanning_tree = kruskal(vertices, edges, edgelist1);
    
    sort(edgelist2.begin(), edgelist2.end(), weight_comp);
    struct Kruskal_Result minimum_spanning_tree = kruskal(vertices, edges, edgelist2);
    
    upper_bound = maximum_spanning_tree.profit_sum/minimum_spanning_tree.weight_sum;
    lower_bound = 1;
    
    while(upper_bound-lower_bound > threshold) {
    
        c = (upper_bound+lower_bound)/2.0;
        for (int i=0; i<edges; i++) {
            edgelist[i][0] = edgelist[i][3] - c*edgelist[i][4];
        }
        
        sort(edgelist.begin(), edgelist.end(), binary_weight_comp);

        result = kruskal(vertices, edges, edgelist);
        if (result.profit_sum - c*result.weight_sum >= 0) 
            lower_bound = c;
        else 
            upper_bound = c;
    }
    result.profit_sum = result.profit_sum/result.gcd_of_sums;
    result.weight_sum = result.weight_sum/result.gcd_of_sums;
    cout << (int)result.profit_sum << ' ' << (int)result.weight_sum << endl;
    
}

int main () {
    int vertices, edges;
    vector<vector<double>> edgelist;
    cin >> vertices >> edges;
    double u, v, profit, weight;

    for (int i=0; i<edges; i++) {
        cin >> u >> v >> profit >> weight;
        edgelist.push_back({0, u, v, profit, weight});
    }

    MaximumRatioTree(vertices, edges, edgelist);
    return 0;
}