#include <iostream> 
#include <iterator>
#include <ostream>
#include <vector>
#include <map>
#include <utility>
#include <list>

using namespace std;

struct node{
    list<int> next;
    int prev = 0;
    int speed = 0;
    int load = 0;
};

void DFS(int u, int N, int d, int best_yet, bool visited[], map<pair<int, int>, int> edges, vector<node> nodes, int distances[], int faster[], int total_time[]){
    cout << "Got into DFS for node: " << u << endl;
    //cout << "Contexts of node " << u << "are: "; 
    /*for (const auto& next : nodes[u].next) {
            std::cout << next << " ";
    }*/
    visited[u] = true;
    best_yet = u;
    //cout << "All cool";
    for (auto& i : nodes[u].next) {
        cout << "Currently checking " << u << "'s neighboor: " << i << endl;
        if (!visited[i]) {
            cout << "He's not visited" << endl;
            distances[i] = d+edges[make_pair(u,i)];
            cout << "His total distance from the 1 is: " << distances[i] << endl;
            total_time[i] = distances[i]*nodes[i].speed+nodes[i].load;
            if (best_yet)
            cout << "His total time from the 1 is: " << total_time[i] << endl;
            cout << "The time of the node to cross the rest of the road is: " << total_time[i]-nodes[i].speed*edges[make_pair(u,i)]-nodes[i].load << endl; 

            if (u == 1 ) {
                best_yet = i;
                cout << "I'm an immediate neighboor of 1" << endl;
                faster[i] = i;
            }
            else if (nodes[i].speed*distances[nodes[i].prev] >= total_time[nodes[i].prev]){
                cout << "Turns out the previous guy, " << nodes[i].prev << ", was faster" << endl;
                faster[i] = nodes[i].prev;
            }
            else { 
                int j=i;
                faster[i] = i;
                cout << "Turns out the previous guy, " << nodes[i].prev << ", was slower" << endl;
                while (j-1 >= 1) {
                    j = faster[nodes[j].prev];
                    cout << "So now I'm gonna check his previous' faster: " << j << endl;
                    if (nodes[i].speed*distances[j] >= total_time[faster[j]])
                        faster[i] = j;
                    if (j == 2){ 
                        break;
                    }
                } 
            }
            DFS(i, N, best_yet, distances[i], visited, edges, nodes, distances, faster, total_time);
        }
    }
}



int main() {
    int N; 
    map<pair<int, int>, int> edges;
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
        int s, d, dis;
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


    for (const auto& pair : edges) {
        std::cout << "{" << pair.first.first << ", " << pair.first.second << "}: " << pair.second << std::endl;
    }

    // Print the contents of the vector of nodes
    std::cout << "Nodes Contents:" << std::endl;
    int counter = 0;
    for (const auto& n : nodes) {
        std::cout << "Node:" << counter << std::endl;
        std::cout << "  Prev: " << n.prev << std::endl;
        std::cout << "  Speed: " << n.speed << std::endl;
        std::cout << "  Load: " << n.load << std::endl;
        std::cout << "  Next: ";
        for (const auto& next : n.next) {
            std::cout << next << " ";
        }
        std::cout << std::endl;
        counter++;
    }

    int distances[N+1];
    int total_time[N+1];
    int faster[N+1];
     
    DFS(1, N, 0, 0, visited, edges, nodes, distances, faster, total_time);
    
    cout << "Made it out of the DFS" << endl;

    for(int j=1; j<=N; j++) {
        cout << "The total time of node " << j << " is: " << total_time[j] << endl;
    }

    cout << "The faster array looks like this: " << endl; 
    
    for(int j=1; j<=N; j++) {
        cout << "The fastest previous of node " << j << " is: " << faster[j] << endl;
    }



    return 0;
}

