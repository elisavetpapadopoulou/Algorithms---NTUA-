#include <iostream>
#include <vector>
#include <deque>
using namespace std;

typedef long long ll;

struct Line {
    ll m, c;
    ll eval(ll x) { return m*x + c; }
    ll intersect(Line other) {
        return (other.c - c + m - other.m - 1) / (m - other.m);
    }
};

struct CHT {
    deque<Line> hull;

    void insert(Line line) {
        while (hull.size() >= 2 && 
               hull[hull.size() - 2].intersect(hull[hull.size() - 1]) >= hull[hull.size() - 1].intersect(line)) {
            hull.pop_back();
        }
        hull.push_back(line);
    }

    ll query(ll x) {
        while (hull.size() >= 2 && hull[1].intersect(hull[0]) <= x) {
            hull.pop_front();
        }
        return hull[0].eval(x);
    }
};

struct City {
    ll distanceToNext;
    ll loadTime, speed;
};

int main() {
    int n;
    cin >> n;

    vector<City> cities(n);
    for (int i = 0; i < n - 1; ++i) {
        cin >> cities[i].distanceToNext;
    }

    for (int i = 1; i < n; ++i) {
        cin >> cities[i].loadTime >> cities[i].speed;
    }

    CHT cht;
    vector<ll> dp(n);
    dp[n - 1] = 0; // Base case: last city

    for (int i = n - 2; i >= 0; --i) {
        cht.insert({-cities[i + 1].speed, dp[i + 1]});
        dp[i] = cht.query(cities[i].loadTime) + cities[i].distanceToNext * cities[i].speed;
    }

    for (int i = 1; i < n; ++i) {
        cout << dp[i] << " ";
    }
    cout << endl;

    return 0;
}

