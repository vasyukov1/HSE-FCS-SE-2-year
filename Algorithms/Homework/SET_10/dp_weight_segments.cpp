#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
using std::cin;
using std::cout;
using std::vector;

struct Segment {
    double start;
    double end;
    double weight;
};

int findNonconflictSegment(const vector<Segment>& segments, int i) {
    int low = 0;
    int high = i - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (segments[mid].end <= segments[i].start) {
            if (mid + 1 < i && segments[mid + 1].end <= segments[i].start) {
                low = mid + 1;
            } else {
                return mid;
            }
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

int main() {
    int n;
    cin >> n;

    if (n == 0) {
        cout << "0.0000\n";
        return 0;
    }

    vector<Segment> segments(n);

    double s, e, w;
    for (int i = 0; i < n; ++i) {
        cin >> s >> e >> w;
        segments[i] = {s, e, w};
    }

    std::sort(segments.begin(), segments.end(), [](const Segment& lhs, const Segment& rhs) {
        if (lhs.end == rhs.end) {
            return lhs.weight > rhs.weight;
        }
        return lhs.end < rhs.end;
    });

    vector dp(n, 0.0);
    dp[0] = segments[0].weight;

    for (int i = 1; i < n; ++i) {
        int index = findNonconflictSegment(segments, i);
        if (index == -1) {
            dp[i] = std::max(dp[i - 1], segments[i].weight);
        } else {
            dp[i] = std::max(dp[i - 1], dp[index] + segments[i].weight);
        }
    }


    cout << std::fixed << std::setprecision(4) << dp[n - 1] << '\n';
    return 0;
}
