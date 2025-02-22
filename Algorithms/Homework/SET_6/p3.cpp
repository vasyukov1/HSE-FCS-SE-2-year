#include <iostream>
#include <vector>
#include <map>

class DSU {
    std::vector<int> parent;
    std::vector<int> rank;

public:
    DSU(int n) {
        parent.resize(n + 1);
        rank.resize(n + 1, 1);
        for (int i = 1; i <= n; ++i) {
            parent[i] = i;
        }
    }

    int find(int v) {
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

};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, k;
    int x, y;
    std::cin >> n >> m >> k;

    std::vector<std::pair<int, int>> edges(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> edges[i].first >> edges[i].second;
    }

    std::vector<std::pair<std::string, std::pair<int, int>>> operations(k);
    std::map<std::pair<int, int>, bool> is_deleted;
    std::string operation;
    for (int i = 0; i < k; ++i) {
        std::cin >> operation >> x >> y;
        operations[i] = {operation, {x, y}};
        if (operation == "cut") {
            is_deleted[{x, y}] = true;
            is_deleted[{y, x}] = true;
        }
    }

    DSU dsu(n);
    for (auto& [v, u] : edges) {
        if (!is_deleted[{v, u}]) {
            dsu.unite(v, u);
        }
    }

    std::vector<std::string> result;
    std::reverse(operations.begin(), operations.end());
    for (auto& [operation, edge] : operations) {
        x = edge.first;
        y = edge.second;
        if (operation == "ask") {
            result.push_back(dsu.find(x) == dsu.find(y) ? "YES" : "NO");
        } else {
            dsu.unite(x, y);
        }
    }

    std::reverse(result.begin(), result.end());
    for (const auto& res : result) {
        std::cout << res << '\n';
    }

    return 0;
}

