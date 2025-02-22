#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

class Prim {
    std::vector<std::vector<int>> edges;
    std::vector<std::vector<int>> minEdges;
    std::set<int> visited;
    int size;

    void addMinEdges(int v) {
        for (size_t i = 0; i < edges.size(); ++i) {
            if (edges[i][1] == v && !visited.contains(edges[i][2]) ||
                edges[i][2] == v && !visited.contains(edges[i][1])) {
                minEdges.push_back(edges[i]);
                }
        }
    }

public:
    Prim(const int n) : size(n) {}

    void addEdge(const int a, const int b, const int c) {
        edges.push_back({c, a, b});
    }

    int findMin(int start) {
        int result = 0;
        visited.insert(start);
        addMinEdges(start);

        while (visited.size() != size) {
            std::sort(minEdges.begin(), minEdges.end());
            result += minEdges[0][0];

            if (!visited.contains(minEdges[0][1])) {
                visited.insert(minEdges[0][1]);
                addMinEdges(minEdges[0][1]);
            } else {
                visited.insert(minEdges[0][2]);
                addMinEdges(minEdges[0][2]);
            }

            std::cout << minEdges[0][0] << ' ' << minEdges[0][1] << ' ' << minEdges[0][2] << '\n';
            minEdges.erase(minEdges.begin());
        }

        return result;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    Prim prim(n);
    int a, b, c;
    for (int i = 0; i < m; ++i) {
        std::cin >> a >> b >> c;
        prim.addEdge(a, b, c);
    }

    int res1 = prim.findMin(a);
    std::cout << res1;
    return 0;
}