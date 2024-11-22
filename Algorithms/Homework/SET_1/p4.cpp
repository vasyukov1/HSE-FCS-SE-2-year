#include <iostream>
#include <vector>
#include <deque>
#include <set>
using std::cin;
using std::cout;
using std::vector;
using std::deque;
using std::set;

int distance(const vector<int>& a, const deque<int>& b);

int main() {
    int n;
    cin >> n;

    vector<int> a(n);
    deque<int> b(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        b[i] = a[i];
    }

    set<deque<int>> combinations;
    int max = 0;
    int count = 0;


    for (int j = 0; j < n; ++j) {
        int el = b.back();
        b.push_front(el);
        b.pop_back();
        if (!combinations.contains(b)) {
            combinations.insert(b);
            int hamming = distance(a, b);
            if (hamming > max) {
                max = hamming;
                count = 1;
            } else if (hamming == max) {
                ++count;
            }
        }
    }
    cout << count;
    return 0;
}

int distance(const vector<int>& a, const deque<int>& b) {
    int counter = 0;
    int n = a.size();
    for (int i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            ++counter;
        }
    }
    return counter;
}