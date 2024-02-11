#include <iostream>
 using namespace std;


 bool IsBigger (int PosMed, int K, int N, int C[]){
    int Distribution[N], i; 
    for (i=0; i<N; i++){
        if (C[i] >= PosMed) {
            Distribution[i] = 1;
        }
        else 
            Distribution[i] = -1;
        if (i != 0)
            Distribution[i] += Distribution[i-1]; 
    }
    /*cout << "This is the Distribution table for Possible Med = " << PosMed  << endl;;
    cout << "[";
    for (i=0; i<N; i++){
        cout << Distribution[i];
        if (i != N-1)
            cout << ",";
        else 
            cout << "]" << endl;
    }*/

    int MinYet = 0;
    int MaxYet = Distribution[K-1];
    
    for(i = K; i<N; i++){
        MinYet = min(MinYet, Distribution[i-K]);
        MaxYet = max(MaxYet, Distribution[i] - MinYet);
    }
    
    if (MaxYet > 0)
        return true;
    else 
        return false;
 }

 int MaximumMedian(int C[], int N, int K){
    int left, right, med, MaxMed;
    
    left = 1;
    right = N+1; 
    while (left < right){
        med = (left+right)/2;
        //cout << "Left is " << left << ", right is " << right << endl;
        if (IsBigger(med, K, N, C)){
            left = med + 1;
            MaxMed = med;  
        }
        else {
            right = med;
        }
       // cout << "This is the Maximum Median so far, with left = " << left << ", right = " << right << ": " << MaxMed << endl;
    }
    return MaxMed;
 }   
 
 int main() {
    int N, K, i;
    cin >> N >> K;
    
    int C[N];

    for (i=0; i<N; i++){
        cin >> C[i];
    }
    cout << MaximumMedian(C, N, K) << endl;
    return 0;
 }