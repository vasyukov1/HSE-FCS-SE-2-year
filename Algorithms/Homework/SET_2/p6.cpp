#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <map>
using std::cin;
using std::cout;
using std::vector;
using std::pair;
using std::map;

struct Point{
    int x;
    int y;
    int z;

    Point(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}

    [[nodiscard]] double distation(const Point& other) const {
        return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2) +  pow(z - other.z, 2));
    }

    bool operator<(const Point& other) const {
        return x < other.x || (x == other.x && y < other.y) || (x == other.x && y == other.y && z < other.z);
    }
};

pair<int, int> findMinDistation(vector<Point>& points);

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    cin >> n;
    vector<Point> points;

    for (size_t i = 0; i < n; ++i) {
        int x, y, z;
        cin >> x >> y >> z;
        points.emplace_back(x, y, z);
    }

    vector<Point> v2(points);
    pair<int, int> ans = findMinDistation(points);

    cout << std::setprecision(10) << v2[ans.first].distation(v2[ans.second]) << '\n';
    cout << ans.first + 1 << ' ' << ans.second + 1;
    return 0;
}


pair<int, int> findMinDistation(vector<Point>& points) {
    size_t n = points.size();
    if (n <= 3) {
        pair<int, int> ans = {0, n - 1};
        for (size_t i = 1; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                double dist = points[i].distation(points[j]);
                if (dist < points[ans.first].distation(points[ans.second])) {
                    ans = std::make_pair(i, j);
                }
            }
        }
        return ans;
    }

    map<Point, int> archive;
    for (int i = 0; i < n; ++i) {
        archive[points[i]] = i;
    }

    std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y) || (a.x == b.x && a.y == b.y && a.z < b.z);
    });

    int mid = points[n / 2].x;
    vector<Point> points1, points2;
    for (size_t i = 0; i < n / 2; ++i) {
        points1.push_back(points[i]);
        points2.push_back(points[n / 2 + i]);
    }

    pair<int, int> res1 = findMinDistation(points1);
    pair<int, int> res2 = findMinDistation(points2);
    if (res1.first == res1.second) {
        res1 = std::make_pair(0, n - 1);
    }
    if (res2.first == res2.second) {
        res2 = std::make_pair(0, (n - 1) / 2);
    }
    res2 = std::make_pair(res2.first + n / 2, res2.second + n / 2);

    double delta = std::min(
        points[res1.first].distation(points[res1.second]),
        points[res2.first].distation(points[res2.second])
    );
    pair<int, int> res = delta == points[res1.first].distation(points[res1.second]) ? res1 : res2;


    vector<pair<Point, int>> center;
    for (size_t i = 0; i < n; ++i) {
        if (std::abs(points[i].x - mid) < delta) {
            center.push_back(std::make_pair(points[i], i));
        }
    }

    int minY = INT_MAX;
    int maxY = INT_MIN;
    for (size_t i = 0; i < center.size(); ++i) {
        minY = std::min(minY, center[i].first.y);
        maxY = std::max(maxY, center[i].first.y);
    }

    vector<vector<pair<Point, int>>> lines(static_cast<int>((maxY - minY + delta) / delta));
    for (size_t i = 0; i < center.size(); ++i) {
        lines[static_cast<int>((center[i].first.y - minY) / delta)].push_back(center[i]);
    }

    for (int i = 0; i < lines.size(); ++i) {
        for (int j = 0; j < lines[i].size(); ++j) {
            for (int k = std::max(0, j - 17); k < j; ++k) {
                if (lines[i][j].first.distation(lines[i][k].first) < points[res.first].distation(points[res.second])) {
                    res = std::make_pair(lines[i][j].second, lines[i][k].second);
                }
            }
        }
    }

    return std::make_pair(archive[points[res.first]], archive[points[res.second]]);
}