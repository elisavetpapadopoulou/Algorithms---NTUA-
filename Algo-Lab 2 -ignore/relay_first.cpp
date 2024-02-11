#include <iostream>
#include<stdio.h>
#include <fstream>
#include <bits/stdc++.h>
#include<map>
#include <vector>
#include<string>
using namespace std;
void addEdge(vector<int> adj[], int u, int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);
}

void DFS(int *prev, int v, bool *visited, vector<int> adj[], int* dist_to, map<string, int>& dist_edge, int curr_dist)
{
    visited[v] = true;

    for (auto i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[*i]){
            prev[*i] = v;
            int cost;
            if (*i < v){
                cost = dist_edge[to_string(*i) + "," + to_string(v)];
            }
            else{
                cost = dist_edge[to_string(v) + "," + to_string(*i)];
            }
            dist_to[*i] = curr_dist + cost;
            DFS(prev, *i, visited, adj, dist_to, dist_edge, curr_dist + cost);
        }
}
void DFS_solver(int *prev, int v, bool *visited, vector<int> adj[], int* dist_to, map<string, int>& dist_edge, int *p, int *s, int *min_time){
    cout << "Node " << v << endl;
    visited[v] = true;
    int min_sum;
    //node 0 returns immediately
    if (v==0){
        min_sum=0;
    }	
    //node != 0
    else{
        int s_rate = s[v];
        //initially cost of going all the way to v1 -> find min of all combinations
        min_sum = p[v] + s_rate*dist_to[v];
        cout <<"All the way is:" << min_sum << endl;
        int cost;
        int node = v;

        if (prev[node] < node)
            cost = dist_edge[to_string(prev[node]) + "," + to_string(node)];
        else
            cost = dist_edge[to_string(node) + "," + to_string(prev[node])];

        int curr_cost = cost;

        node = prev[node];
        while(prev[node] != -1){

            int temp = p[v] + curr_cost * s_rate + min_time[node];
            cout << "Stop in " << node << " and then continue: " << temp << endl;


            if (prev[node] < node)
                cost = dist_edge[to_string(prev[node]) + "," + to_string(node)];
            else
                cost = dist_edge[to_string(node) + "," + to_string(prev[node])];

            curr_cost += cost;
            if (temp < min_sum)
                min_sum = temp;
            node = prev[node];
        }}
    min_time[v] = min_sum;
    for (auto i = adj[v].begin(); i != adj[v].end(); ++i){
        if (!visited[*i]){
            cout << "Going to " << *i << endl;
            cout << "---------------------------" << endl;
            DFS_solver(prev, *i, visited, adj, dist_to, dist_edge, p, s, min_time);
        }
    }
}
int main(){
    //reading part below--------------------------
    int N;
    cin >> N;

    int* prev=new int[N];
    prev[0]=-1;

    int* p=new int[N];
    int* s=new int[N];
    p[0]=0;
    s[0]=0;
    vector<int> adj[N];

    //lengths of edges 
    map<string, int> dist_edge;

    for(int i=1; i<N; i++) {
        int* temp=new int[3];
        for (int j=0; j<3; j++){
            cin >> temp[j];
        }
        temp[0] += -1;
        temp[1] += -1;

        addEdge(adj, temp[0], temp[1]);
        if (temp[0] < temp[1]){
            string string_edge = to_string(temp[0]) + "," + to_string(temp[1]);
            dist_edge[string_edge] = temp[2];
        }
        else{
            string string_edge = to_string(temp[1]) + "," + to_string(temp[0]);
            dist_edge[string_edge] = temp[2];
        }
    }
    for(int i=0; i<N-1; i++){
        int* temp=new int[2];
        for (int j=0; j<2; j++){
            cin >> temp[j];
        }
        p[i+1] = temp[0];
        s[i+1] = temp[1];
    }
    //readind part above--------------------------

    bool* visited=new bool[N];
    for(int i=0; i<N; i++){
        visited[i] = false;
    }
    int* dist_to=new int[N];
    dist_to[0] = 0;
    //
    cout<<endl<<"ADJ"<<endl;
    for (int k=0; k<N; k++){
        for (auto i = adj[k].begin(); i != adj[k].end(); ++i){
            cout << *i<<" ";	
        }cout<<endl;}
    //fill dist_to and prev arrays
    DFS(prev,0,visited,adj,dist_to, dist_edge, 0);

    int* min_time=new int[N];
    min_time[0]=0;
    for(int i=0; i<N; i++)
        visited[i] = false;

    //magic!
    DFS_solver(prev, 0, visited, adj, dist_to, dist_edge, p, s, min_time);

    for(int i=1; i<N; i++)
        cout << min_time[i] << " ";
    return 0;
}
