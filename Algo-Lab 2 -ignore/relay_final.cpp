#include <iostream>
#include <ostream>
#include <fstream> 
#include <iterator>
#include <list>
#include <map>
#include <utility>
#include <limits>
#include <algorithm>
#include <vector>
#include <deque>
using namespace std;

struct node{
    list<long long> next;
    long long prev = 0;
    long long speed = 0;
    long long load = 0;
};


void DFS(long long u, long long N, long long d, bool visited[], map<pair<long long, long long>, long long>& edges, vector<node>& nodes, long long distances[], long long faster[], long long total_time[]){
    visited[u] = true;
    for (auto& i : nodes[u].next) {
        if (!visited[i]) {
            faster[i] = i;
            distances[i] = d+edges[make_pair(u,i)];
            total_time[i] = distances[i]*nodes[i].speed+nodes[i].load;

            long long j = i; 
            j = nodes[j].prev;
            long long cost = edges[make_pair(j, i)];
            while (nodes[j].prev >= 1) {
                //Possibility of stopping at j node. 
                long long temp = nodes[i].load + nodes[i].speed*cost + total_time[j];
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



struct Convex_Hull_Trick{

    struct line {
        long long a;
        long long b;

        line(double a, long long b) : a(a), b(b) {}
        long long evaluate(long long x_axis) {return a*x_axis + b;}
        long double intersect(line y_axis) {return (long double)(y_axis.b - b) / (a - y_axis.a);}
    };

    deque<pair<line, long long>> que;

    void addLine(long long a, long long b) {
        line added_line(a, b);

        while (!que.empty() && que.back().second >= que.back().first.intersect(added_line)) que.pop_back();

        if (que.empty()) {
            que.emplace_back(added_line, 0);
            return;
        }

        que.emplace_back(added_line, que.back().first.intersect(added_line));
    }

    long long searchLine(long long x_axis) {
        long long res;

        if (x_axis >= que.back().second) {
            return que.back().first.evaluate(x_axis);
        }

         long long maximum = que.size() - 1, minimum = 0;
         long long mid = (minimum + maximum)/2;
        
         while (minimum < maximum) {
             if (que[mid].second < x_axis) {
                 res = que[mid].first.evaluate(x_axis);
                 minimum = mid + 1;
                 mid = (minimum + maximum)/2;
             } else {
                 maximum = mid;
                 mid = (minimum + maximum)/2;
             }
         }

        return res;
    }

};


struct Town { 
    vector<int> neighbors;
    map<int,int> d;
    int loading_time,speed;
};


void Solution_for_big_N(std::vector<long long> &dp, int &N) {
    for (int i = 2; i <= N; i++) {
        std::cout << dp[i];
        if (i != N) {
            std::cout << " ";
        } else {
            std::cout << std::endl;
        }
    }
}
int main() {
    int N;
    std::cin >> N;


    if (N > 8000){
        vector<long long> dp(N+1,0);
        vector<Town> Nodes_for_big_N(N+1);

        for (int i = 2; i <= N; i++) {
            int u, v, d;
            std::cin >> u >> v >> d;

            Nodes_for_big_N[u].neighbors.push_back(v);
            Nodes_for_big_N[v].neighbors.push_back(u);
            Nodes_for_big_N[u].d[v] = d;
            Nodes_for_big_N[v].d[u] = d;
        }

        for (int i = 2; i <= N; i++) {
            int loading_time, speed;
            std::cin >> loading_time >> speed;
            Nodes_for_big_N[i].loading_time = loading_time;
            Nodes_for_big_N[i].speed = speed;
        }

        for (auto neighbor : Nodes_for_big_N[1].neighbors) {

            int previous = 1;
            int current = neighbor;
            long long traversed = 0;
            int next_stop;
            if (Nodes_for_big_N[current].neighbors.size() != 2) {dp[current] = Nodes_for_big_N[current].loading_time + Nodes_for_big_N[current].speed*Nodes_for_big_N[previous].d[current];}
            Convex_Hull_Trick CHT;
            CHT.addLine(0,0);

            while (Nodes_for_big_N[current].neighbors.size() == 2) {

                for (auto next: Nodes_for_big_N[current].neighbors) {
                    if (next!=previous) { 
                        dp[current] = CHT.searchLine(Nodes_for_big_N[current].speed)+Nodes_for_big_N[current].loading_time+Nodes_for_big_N[current].speed*(traversed+Nodes_for_big_N[current].d[previous]);
                        CHT.addLine(-(traversed + Nodes_for_big_N[current].d[previous]),dp[current]);
                        traversed += Nodes_for_big_N[current].d[previous];
                        next_stop = next;
                    }
                }
                previous = current;
                current = next_stop;
            }
            dp[current] = CHT.searchLine(Nodes_for_big_N[current].speed)+Nodes_for_big_N[current].speed*(traversed + Nodes_for_big_N[current].d[previous])+Nodes_for_big_N[current].loading_time;
        }
        Solution_for_big_N(dp, N);
    }
    else {
        map<pair<long long, long long>, long long> edges;
        vector<node> nodes; 

        bool visited[N+1]; 
        for (int i=0; i<N+1; i++) {
            visited[i] = false;
        }

        for(int i=0; i <=N; i++){
            node n;
            nodes.push_back(n);
        }

        for(int i=1; i <N; i++){
            long long s, d, dis;
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



        long long distances[N+1];
        long long total_time[N+1];
        long long faster[N+1];

        DFS(1, N, 0, visited, edges, nodes, distances, faster, total_time);


        for(int j=2; j<=N; j++) {
            if (j != 2) 
                cout << " ";
            cout << total_time[j]; 
        }

        cout << endl;



    } 
    return 0;
}


