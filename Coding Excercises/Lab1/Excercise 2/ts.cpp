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
    double gcd_of_sums;
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


struct Kruskal_Result kruskal(int vertices, int edges, vector<vector<double> > edgelist) { 
    struct Kruskal_Result ans;
    DSU s(vertices);
    for (int i=0; i<edges; i++) {            
        int w = edgelist[i][0]; 
        int x = edgelist[i][1]; 
        int y = edgelist[i][2]; 

        // Take this edge in MST if it does 
        // not forms a cycle 
        if (s.find(x) != s.find(y)) { 
            s.unite(x, y); 
            ans.profit_sum += edgelist[i][3];
            ans.weight_sum += edgelist[i][4];
            cout << x << " -- " << y << " == " << w << endl; 
        } 
    } 
    return ans;
}

int MaximumRatioTree(int vertices, int edges, vector<vector<double>> edgelist) {
    cout << "I'm here" << endl;
    double lower_bound = 1;
    double threshold = 0.01; 
    Kruskal_Result ans;

    vector<vector<double> > edgelist1 = edgelist;
    vector<vector<double> > edgelist2 = edgelist;

    sort(edgelist1.begin(), edgelist1.end(), profit_comp);
    Kruskal_Result upper_kr = kruskal(vertices, edges, edgelist1);

    sort(edgelist2.begin(), edgelist2.end(), weight_comp);
    Kruskal_Result lower_kr = kruskal(vertices, edges, edgelist2);

    double upper_bound = (double)upper_kr.profit_sum/lower_kr.weight_sum;

    while(upper_bound-lower_bound >= threshold) {
        double c = (upper_bound+lower_bound)/2;
        
        for (int i=0; i<edges; i++){
            edgelist[i][0] = edgelist[i][3] - c*edgelist[i][4];
            cout << edgelist[i][0] << endl;
        }        
        
        sort(edgelist.begin(), edgelist.end(), binary_weight_comp);
        cout << "Following are the edges in the "
                "constructed MST"
             << endl; 
        ans = kruskal(vertices, edges, edgelist);
        ans.gcd_of_sums = gcd((int)ans.profit_sum, (int)ans.weight_sum);
        cout << ans.profit_sum/ans.gcd_of_sums << " " << ans.weight_sum/ans.gcd_of_sums << endl;
        
        if (ans.profit_sum - c*ans.weight_sum >= 0) {
            lower_bound = c;
        }
        else {
            upper_bound = c;
        }
    }
    return 0;
    //Kruskal with Binary_weights for criteria. If in the end the total profit minus c times 
    // the total weight is >= 0, make c bigger, cause it means that there is a better ratio to be achieved. 
}

int main() {
    int vertices, edges;

    cin >> vertices >> edges;

    vector<vector<double>> edgelist;

    for (int i = 0; i < edges; ++i) {
        double u, v, profit, weight;
        cin >> u >> v >> profit >> weight;
        edgelist.push_back({0, u, v, profit, weight});
    }

    int result =  MaximumRatioTree(vertices, edges, edgelist);
    return 0;
}
