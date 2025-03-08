#include <iostream>
#include <vector>
#include <queue>

class HopcroftKarp {
private:
    std::vector<std::vector<int>> graph;

    std::vector<int> usedA;
    std::vector<int> usedB;
    std::vector<int> dist;
    int n;

    bool bfs() {
        std::queue<int> q;
        for (int i = 0; i < n; ++i) {
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
            if (usedB[v] == -1 || (dist[usedB[v]] == dist[u] + 1 && dfs(usedB[v]))) {
                usedA[u] = v;
                usedB[v] = u;
                return true;
            }
        }
        return false;
    }

public:
    HopcroftKarp(const int n) : graph(n), dist(n, INT_MAX), n(n) {
        usedA.resize(n, -1);
        usedB.resize(n, -1);
    }

    void addEdge(const int u, const int v) {
        graph[u].push_back(v);
    }

    int matching() {
        int matches = 0;
        while (bfs()) {
            for (int i = 0; i < n; ++i) {
                if (usedA[i] == -1 && dfs(i)) {
                    ++matches;
                }
            }
        }
        return matches;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    int a, b;
    std::cin >> n >> m >> a >> b;

    HopcroftKarp hk(n * m);
    int countTile = 0;
    std::vector tiles(n, std::vector(m, -1));
    std::string s;
    int counter = 0;
    for (int i = 0; i < n; ++i) {
        std::cin >> s;
        for (int j = 0; j < m; ++j) {
            if (s[j] == '*') {
                tiles[i][j] = counter++;
                ++countTile;

                if (i > 0 && tiles[i - 1][j] != -1) {
                    int x = tiles[i][j];
                    int y = tiles[i - 1][j];
                    if ((i + j) % 2 != 0) {
                        std::swap(x, y);
                    }
                    hk.addEdge(x, y);
                }

                if (i + 1 < n && tiles[i + 1][j] != -1) {
                    int x = tiles[i][j];
                    int y = tiles[i + 1][j];
                    if ((i + j) % 2 != 0) {
                        std::swap(x, y);
                    }
                    hk.addEdge(x, y);
                }

                if (j > 0 && tiles[i][j - 1] != -1) {
                    int x = tiles[i][j];
                    int y = tiles[i][j - 1];
                    if ((i + j) % 2 != 0) {
                        std::swap(x, y);
                    }
                    hk.addEdge(x, y);
                }

                if (j + 1 < m && tiles[i][j + 1] != -1) {
                    int x = tiles[i][j];
                    int y = tiles[i][j + 1];
                    if ((i + j) % 2 != 0) {
                        std::swap(x, y);
                    }
                    hk.addEdge(x, y);
                }
            }
        }
    }

    const int matches = hk.matching();
    const int resMix = matches * a + (countTile - matches * 2) * b;
    const int resB = countTile * b;
    std::cout << (resMix < resB ? resMix : resB) << '\n';

    return 0;
}
