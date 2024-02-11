#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <climits>

using namespace std;

struct Edge {
    int to, travelTime;
};

int main() {
    int N, M, A, B, T, K;
    cin >> N >> M >> A >> B >> T >> K;


    vector<int> presidentRoute(K);
    for (int i = 0; i < K; ++i) {
        cin >> presidentRoute[i];
    }

    vector<vector<Edge>> graph(N + 1);
    for (int i = 0; i < M; ++i) {
        int u, v, time;
        cin >> u >> v >> time;
        graph[u].push_back({v, time});
        graph[v].push_back({u, time});
    }
    // Map to store the times at which each road is blocked due to president's travel
    map<pair<int, int>, pair<int, int>> blockedTimes;
    int currentTime = 0;
    for (int i = 0; i < K - 1; ++i) {
        int city1 = presidentRoute[i];
        int city2 = presidentRoute[i + 1];
        for (Edge& e : graph[city1]) {
            if (e.to == city2) {
                blockedTimes[{city1, city2}] = {currentTime, currentTime + e.travelTime - 1};
                blockedTimes[{city2, city1}] = {currentTime, currentTime + e.travelTime - 1};
                currentTime += e.travelTime;
                break;
            }
        }
    }

    // Modified Dijkstra's algorithm with waiting
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> dist(N + 1, INT_MAX);
    pq.push({T, A});  // Start from city A at time T
    dist[A] = T;

    while (!pq.empty()) {
        int time = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (u == B) {
            cout << time - T << endl;
            return 0;
        }
        for (Edge& e : graph[u]) {
            int v = e.to;
            int arrivalTime = time + e.travelTime;
            //if (v == B) {
                //cout << "I am node " << v << " and the arrival time of the prev guy so far is " << arrivalTime << endl;
            //}

            // Check if the road is blocked and wait if necessary
            auto it = blockedTimes.find({u, v});
            if (it != blockedTimes.end()) {
                int startBlock = it->second.first;
                int endBlock = it->second.second;
                //cout << "Found out this road is blocked from minute " << startBlock << " till minute " << endBlock << endl;
                if (time <= endBlock && time >= startBlock) {
                    // Wait until the road reopens
                    arrivalTime = e.travelTime + endBlock + 1;
                }
            }

            //cout << "Could arrive in node " << v << " in time " << arrivalTime << endl;
            
            if (arrivalTime < dist[v]) {
                dist[v] = arrivalTime;
                pq.push({arrivalTime, v});
            }
        }

    }
    cout << "Route not possible" << endl;
    return 0;
}
