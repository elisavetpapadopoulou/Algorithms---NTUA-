#include <fstream>
#include <iostream> 
#include <iterator>
#include <ostream>
#include <vector>
#include <map>
#include <utility>
#include <list>

using namespace std;

struct node{
    list<long  long int> next;
    long long int prev = 0;
    long long int speed = 0;
    long long int load = 0;
};


void DFS(long long int u, long long int N, long long int d, bool visited[], map<pair<long long int, long long int>, long long int>& edges, vector<node>& nodes, long long int distances[], long long int faster[], long long int total_time[]){
    visited[u] = true;
    for (auto& i : nodes[u].next) {
        if (!visited[i]) {
            faster[i] = i;
            distances[i] = d+edges[make_pair(u,i)];
            total_time[i] = distances[i]*nodes[i].speed+nodes[i].load;
     
            long long int j = i; 
            j = nodes[j].prev;
            long long int cost = edges[make_pair(j, i)];
            while (nodes[j].prev >= 1) {
                //Possibility of stopping at j node. 
                long long int temp = nodes[i].load + nodes[i].speed*cost + total_time[j];
                cost += edges[make_pair(nodes[j].prev, j)];
                if (temp < total_time[i]) {
                    faster[i] = j;
                    total_time[i] = temp;
                }

                j = nodes[j].prev;

            }

            DFS(i, N, distances[i], visited, edges, nodes, distances, faster, total_time);
        }
    }
}



int main() {
    long long int N; 
    map<pair<long long int, long long int>, long long int> edges;
    vector<node> nodes; 

    cin >> N;

    bool visited[N+1]; 
    for (int i=0; i<N+1; i++) {
        visited[i] = false;
    }

    for(int i=0; i <=N; i++){
        node n;
        nodes.push_back(n);
    }

    for(int i=1; i <N; i++){
        long long int s, d, dis;
        cin >> s >> d >> dis;
               
        if (s > d) {
            edges[make_pair(d,s)] = dis;
            nodes[d].next.push_back(s);
            nodes[s].prev = d;
        }
        else {
            edges[make_pair(s,d)] = dis;
            nodes[s].next.push_back(d);
            nodes[d].prev = s;
        }
    }
   
    for (int i=2; i<=N; i++) {
        cin >> nodes[i].load >> nodes[i].speed;
    }



    long long int distances[N+1];
    long long int total_time[N+1];
    long long int faster[N+1];
     
    DFS(1, N, 0, visited, edges, nodes, distances, faster, total_time);
    

    for(int j=2; j<=N; j++) {
        if (j != 2) 
            cout << " ";
        cout << total_time[j]; 
    }

    cout << endl;




    return 0;
}

