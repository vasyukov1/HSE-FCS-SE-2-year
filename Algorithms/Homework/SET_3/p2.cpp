#include <iostream>
#include <vector>
using std::cout;
using std::cin;
using std::vector;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr); 

    int star;
    int min_star = INT_MAX;
    int max_star = INT_MIN;

    int n;
    cin >> n;
    vector<int> stars;
    
    for (int i = 0; i < n; ++i) {
        cin >> star;
        stars.push_back(star);
        if (star > max_star) {
            max_star = star;
        }
        if (star < min_star) {
            min_star = star;
        }
    }

    vector<int> counter (max_star - min_star + 1, 0);
    for (int i = 0; i < n; ++i) {
        ++counter[stars[i] - min_star];
    }

    for (int i = 0; i < counter.size(); ++i) {
        for (int j = 0; j < counter[i]; ++j) {
            cout << min_star + i << ' ';
        }
    }

    return 0;
}