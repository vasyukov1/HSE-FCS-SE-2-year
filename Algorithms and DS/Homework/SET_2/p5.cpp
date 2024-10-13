#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using std::cin;
using std::cout;
using std::vector;
using std::pair;

int distance(pair<int, int> x, pair<int, int> y);
int find(vector<pair<int, int>>& dots);

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    vector<pair<int, int>> dots;
    int x, y;
    while (cin >> x >> y) {
        dots.emplace_back(x, y);
    }
    cout << find(dots);
    return 0;
}

int distance(pair<int, int> x, pair<int, int> y) {
    double a = std::pow(x.first - y.first, 2);
    double b = std::pow(x.second - y.second, 2);
    return std::sqrt(a + b);
}

int find(vector<pair<int, int>>& dots) {
    std::sort(dots.begin(), dots.end());
    size_t n = dots.size();

    if (n <= 2) {
        int ans = INT_MAX;
        for (int i = 1; i < n; ++i) {
            ans = std::min(ans, distance(dots[i], dots[i - 1]));
        }
        return ans;
    }

    vector<pair<int, int>> dots1, dots2;
    for (int i = 0; i < n / 2; ++i) {
        dots1.push_back(dots[i]);
    }
    for (int i = n / 2; i < n; ++i) {
        dots2.push_back(dots[i]);
    }

    int ans1 = find(dots1);
    int ans2 = find(dots2);
    int ans = std::min(ans1, ans2);

    vector<pair<int, int>> center;
    for (int i = 0; i < n; ++i) {
        if (std::abs(dots[n / 2].first - dots[i].first) < ans) {
            center.push_back(dots[i]);
        }
    }

    std::sort(center.begin(), center.end(), [](pair<int, int> x, pair<int, int> y) {
        return x.second < y.second;
    });

    for (int i = 1; i < center.size(); ++i) {
        for (int j = std::max(0, i - 6); j < i; ++j) {
            ans = std::min(ans, distance(center[i], center[j]));
        }
    }

    return ans;
}