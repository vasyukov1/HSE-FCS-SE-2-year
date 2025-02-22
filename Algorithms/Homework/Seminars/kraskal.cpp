#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

class DSU {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    explicit DSU(const int n) : parent(n), rank(n, 1) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    int find(const int v) {
        if (parent[v] == v) {
            return v;
        }
        return parent[v] = find(parent[v]);
    }

    void unite(int v, int u) {
        int vp = find(v);
        int up = find(u);
        if (vp != up) {
            if (rank[vp] < rank[up]) {
                std::swap(v, u);
            }
            parent[up] = vp;
            if (rank[vp] == rank[up]) {
                ++rank[vp];
            }
        }
    }

    size_t getNumberUniq() {
        std::set<int> scc;
        for (size_t i = 0; i < parent.size(); ++i) {
            scc.insert(find(static_cast<int>(i)));
        }
        return scc.size();
    }

};

class Kraskal {
private:
    using Edge = std::pair<int, std::pair<int, int>>;
    std::vector<Edge> edges;
    int size;
    DSU dsu;

public:
    explicit Kraskal(int n) : size(n), dsu(n) {}

    void addEdge(int a, int b, int c) {
        const Edge edge = {c, {a, b}};
        edges.push_back(edge);
    }

    int findMinPath() {
        int result = 0;
        std::sort(edges.begin(), edges.end());
        while (dsu.getNumberUniq() != 1) {
            if (edges.empty()) {
                return -1;
            }

            Edge edge = edges[0];
            int a = edge.second.first;
            int b = edge.second.second;
            edges.erase(edges.begin());
            if (dsu.find(a) != dsu.find(b)) {
                result += edge.first;
                dsu.unite(a, b);
            }
        }
        return result;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    Kraskal kraskal(n);
    int a, b, c;
    for (int i = 0; i < m; ++i) {
        std::cin >> a >> b >> c;
        kraskal.addEdge(a - 1, b - 1, c);
    }

    int res1 = kraskal.findMinPath();
    std::cout << res1;
    return 0;
}