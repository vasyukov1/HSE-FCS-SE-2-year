#include <iostream>
#include <vector>
#include <queue>

class HopcroftKarp {
private:
    std::vector<std::vector<int>> graph;

    std::vector<int> usedA;
    std::vector<int> usedB;
    std::vector<int> dist;

    bool bfs() {
        std::queue<int> q;
        for (int i = 1; i < usedA.size(); ++i) {
            if (usedA[i] == -1) {
                dist[i] = 0;
                q.push(i);
            } else {
                dist[i] = INT_MAX;
            }
        }

        bool found = false;
        while (!q.empty()) {
            const int u = q.front();
            q.pop();
            for (const int v : graph[u]) {
                if (usedB[v] == -1) {
                    found = true;
                } else if (dist[usedB[v]] == INT_MAX) {
                    dist[usedB[v]] = dist[u] + 1;
                    q.push(usedB[v]);
                }
            }
        }
        return found;
    }

    bool dfs(const int u) {
        for (const int v : graph[u]) {
            if (usedB[v] == -1 || dist[usedB[v]] == dist[u] + 1 && dfs(usedB[v])) {
                usedA[u] = v;
                usedB[v] = u;
                return true;
            }
        }
        return false;
    }

public:
    HopcroftKarp(const int n, const int m) : graph(n + 1), usedA(n + 1, -1), usedB(m + 1, -1), dist(n + 1, INT_MAX) {}

    void addEdge(const int u, const int v) {
        graph[u].push_back(v);
    }

    int matching() {
        int matches = 0;
        while (bfs()) {
            for (int i = 1; i < graph.size(); ++i) {
                if (usedA[i] == -1 && dfs(i)) {
                    ++matches;
                }
            }
        }
        return matches;
    }

    void printMatches() const {
        for (int i = 1; i < usedA.size(); ++i) {
            if (usedA[i] != -1) {
                std::cout << i << ' ' << usedA[i] << '\n';
            }
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    int m;
    std::cin >> n >> m;

    HopcroftKarp hk(n, m);

    for (int i = 1; i < n + 1; ++i) {
        int b;
        std::cin >> b;
        while (b != 0) {
            hk.addEdge(i, b);
            std::cin >> b;
        }
    }

    const int maxMatching = hk.matching();
    std::cout << maxMatching << '\n';
    hk.printMatches();

    return 0;
}
