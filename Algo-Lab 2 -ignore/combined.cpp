#include <iostream>
#include <map>
#include <vector>
#include <limits>
#include <algorithm>
#include <deque>
using namespace std;

struct Next {
    long int distance_to_child;
    long int child;
};

struct Node {
    long int load;
    long int speed;
    long int parent;
    long int distance_to_parent;
    long int total_distance = 0;
    std::vector<Next> next;
};

void computeTotalDistanceAndOptimalCost(std::vector<bool> &visited,
                                        long int current, std::vector<Node> &nodes,
                                        std::vector<long int> &OPT, long int traversed) {
    visited[current] = true;
    long int total_cost = (current == 1) ? 0 : nodes[current].load;

    if (current != 1) {
        long int current_speed = nodes[current].speed;
        total_cost += current_speed * nodes[current].total_distance;

        long int current_cost = nodes[current].distance_to_parent;
        long int parent_node = nodes[current].parent;

        while (parent_node != -1) {
            long int temp = nodes[current].load + current_cost * current_speed + OPT[parent_node];
            current_cost += nodes[parent_node].distance_to_parent;

            if (temp < total_cost) {
                total_cost = temp;
            }

            parent_node = nodes[parent_node].parent;
        }
    }

    OPT[current] = total_cost;

    for (auto &next : nodes[current].next) {
        if (!visited[next.child]) {
            nodes[next.child].total_distance = traversed + next.distance_to_child;
            computeTotalDistanceAndOptimalCost(visited, next.child, nodes, OPT,
                                                traversed + next.distance_to_child);
        }
    }
}

typedef long long ll;


struct CHT {

    struct Line {

        ll slope;
        ll yIntercept;

        Line(double slope, ll yIntercept) : slope(slope), yIntercept(yIntercept) {}

        ll val(ll x) {
            return slope * x + yIntercept;
        }
        
        long double intersect(Line y) {
            return (long double)(y.yIntercept - yIntercept) / (slope - y.slope);
        }

        // ll intersect(Line y) {
        //     return (y.yIntercept - yIntercept + slope - y.slope - 1) / (slope - y.slope);
        // }
    };

    deque<pair<Line, ll>> dq;

    void insert(ll slope, ll yIntercept) {
        Line newLine(slope, yIntercept);

        while (!dq.empty() && dq.back().second >= dq.back().first.intersect(newLine))
            dq.pop_back();

        if (dq.empty()) {
            dq.emplace_back(newLine, 0);
            return;
        }

        dq.emplace_back(newLine, dq.back().first.intersect(newLine));
    }

    ll query(ll x) {
        ll value;

        if (x >= dq.back().second) {
            return dq.back().first.val(x);
        }

        // ll left = 0;
        // ll right = dq.size() - 1;
        // int mid = (left + right)/2;
        //
        //
        // while (left <= right) {
        //     int mid = left + (right - left) / 2;
        //
        //     if (dq[mid].second < x) {
        //         left = mid + 1;
        //         mid = (left + right)/2;
        //     } else {
        //         right = mid - 1;
        //         mid = (left + right)/2;
        //     }
        // }


        for (auto item: dq) {
            if (x > item.second) {
                value = item.first.val(x);
            }
        }

        return value;
        
    }

};


struct City { 
    vector<int> neighbors;
    map<int,int> distance;
    int load;
    int speed;
};

void printSolution(std::vector<long int> &OPT, int &N) {
    for (int i = 2; i <= N; i++) {
        std::cout << OPT[i];
        if (i != N) {
            std::cout << " ";
        } else {
            std::cout << std::endl;
        }
    }
}

void llprintSolution(std::vector<ll> &OPT, int &N) {
    for (int i = 2; i <= N; i++) {
        std::cout << OPT[i];
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
        vector<ll> OPT(N+1,0);
        vector<City> nodes(N+1);

        for (int i = 2; i <= N; i++) {
            int v1, v2, distance;
            std::cin >> v1 >> v2 >> distance;

            nodes[v1].neighbors.push_back(v2);
            nodes[v2].neighbors.push_back(v1);
            nodes[v1].distance[v2] = distance;
            nodes[v2].distance[v1] = distance;
        }

        for (int i = 2; i <= N; i++) {
            int load, speed;
            std::cin >> load >> speed;
            nodes[i].load = load;
            nodes[i].speed = speed;
        }

        for (auto neighbor : nodes[1].neighbors) {

            int previous = 1;
            int current = neighbor;
            ll total_distance = 0;
            int next_to_go;

            if (nodes[current].neighbors.size() != 2) {
                OPT[current] = nodes[current].load + nodes[current].speed*nodes[previous].distance[current];
            }

            CHT cht;
            cht.insert(0,0);

        while (nodes[current].neighbors.size() == 2) {

            for (auto next: nodes[current].neighbors) {
                if (next != previous) { 
                    OPT[current] = cht.query(nodes[current].speed) +
                        nodes[current].load +
                        nodes[current].speed*(total_distance+nodes[current].distance[previous]);
                    cht.insert(-(total_distance + nodes[current].distance[previous]),OPT[current]);
                    total_distance += nodes[current].distance[previous];
                    next_to_go = next;
                }
            }

            previous = current;
            current = next_to_go;
        }

        // cht.insert(total_distance,OPT[previous]);
        OPT[current] = cht.query(nodes[current].speed) + nodes[current].load +
            nodes[current].speed*(total_distance + nodes[current].distance[previous]);
        }

        llprintSolution(OPT, N);

    }

    else {

        std::vector<Node> nodes(N + 1);
        std::vector<long int> OPT(N + 1, 0);
        std::vector<bool> visited(N + 1, false);

        nodes[1].parent = -1;

        for (int i = 2; i <= N; i++) {
            long int source, destination, distance;
            std::cin >> source >> destination >> distance;

            if (source < destination) {
                nodes[source].next.push_back({ distance, destination });
                nodes[destination].parent = source;
                nodes[destination].distance_to_parent = distance;
            } else {
                nodes[destination].next.push_back({ distance, source });
                nodes[source].parent = destination;
                nodes[source].distance_to_parent = distance;
            }
        }

        for (int i = 2; i <= N; i++) {
            long int load, speed;
            std::cin >> load >> speed;
            nodes[i].load = load;
            nodes[i].speed = speed;
        }

        computeTotalDistanceAndOptimalCost(visited, 1, nodes, OPT, 0);
        printSolution(OPT, N);

    }

    return 0;
}

