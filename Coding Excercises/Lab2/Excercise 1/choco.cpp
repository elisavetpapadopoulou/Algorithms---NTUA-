#include <iostream>
#include <climits>
#include <limits.h>

using namespace std; 

int maximum_profit(int N, int K, int p[]){
    int current_profit[K+1][N+1], i; 

    for(i=0; i<=N; i++){
        current_profit[0][i] = 0;
    }

    for(i=0; i<=K; i++){
        current_profit[i][0] = 0;
    }
    
    for(i=1; i<=K; i++) {
        int previous_maximum = INT_MIN;
        for (int j=1; j<N; j++){
            previous_maximum = max(previous_maximum, current_profit[i-1][j-1]-p[j-1]);
            current_profit[i][j] = max(p[j]+previous_maximum, current_profit[i][j-1]);   
        }
    }
    return current_profit[K][N-1];
}

int main() {
    int N, K, i;
    
    cin >> N >> K;
    int p[N];

    for (i=0; i<N; i++){
        cin >> p[i];
    }

    int total_profit = maximum_profit(N, K, p);
    cout << total_profit << endl;
    return 0; 
}
