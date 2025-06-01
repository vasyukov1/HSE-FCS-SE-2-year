#include <iostream>
#include <vector>
#include <queue>
using std::cin;
using std::cout;
using std::vector;

struct Segment {
    int start;
    int end;
};

struct Compare {
    bool operator()(const Segment& lhs, const Segment& rhs) const {
        return lhs.end > rhs.end;
    }
};

int main() {
    int n, l, r;
    cin >> n;

    std::priority_queue<Segment, vector<Segment>, Compare> s;
    for (int i = 0; i < n; ++i) {
        cin >> l >> r;
        s.push({l, r});
    }

    int pos = -1;
    vector<int> dots;
    for (int i = 0; i < n; ++i) {
        const auto [start, end] = s.top(); s.pop();
        if (start > pos) {
            dots.push_back(end);
            pos = end;
        }
    }

    cout << dots.size() << '\n';
    for (const int dot : dots) {
        cout << dot << ' ';
    }

    return 0;
}
