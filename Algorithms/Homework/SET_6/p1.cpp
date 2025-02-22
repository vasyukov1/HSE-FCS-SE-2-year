#include <iostream>
#include <stack>
#include <vector>

class Kosaraju {
    int n;
    int sccId = 0;
    std::vector<std::vector<int>> graph;
    std::vector<std::vector<int>> rev_graph;
    std::vector<bool> visited;
    std::stack<int> order;
    std::vector<int> scc;

    void DFS1(const int vertex) {
        visited[vertex] = true;
        for (const int neigh : graph[vertex]) {
            if (!visited[neigh]) {
                DFS1(neigh);
            }
        }
        order.push(vertex);
    }

    void DFS2(const int vertex, const int sccId) {
        scc[vertex] = sccId;
        for (const int neigh : rev_graph[vertex]) {
            if (scc[neigh] == -1) {
                DFS2(neigh, sccId);
            }
        }
    }

public:
    explicit Kosaraju(const int n) : n(n), graph(n), rev_graph(n), visited(n, false), scc(n, -1) {}

    void addEdge(const int v, const int u) {
        graph[v].push_back(u);
        rev_graph[u].push_back(v);
    }

    void findSCCs() {
        for (int v = 0; v < n; ++v) {
            if (!visited[v]) {
                DFS1(v);
            }
        }

        while (!order.empty()) {
            const int vertix = order.top();
            order.pop();
            if (scc[vertix] == -1) {
                DFS2(vertix, sccId++);
            }
        }
    }

    std::vector<int> getSCC() {
        return scc;
    }

    [[nodiscard]] int getSCCSize() const {
        return sccId;
    }

};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    Kosaraju kosaraju(n);
    int x, y;
    for (int i = 0; i < m; ++i) {
        std::cin >> x >> y;
        kosaraju.addEdge(x - 1, y - 1);
    }

    kosaraju.findSCCs();
    const int sccSize = kosaraju.getSCCSize();
    const std::vector<int> scc = kosaraju.getSCC();

    std::cout << sccSize << '\n';
    for (int i = 0; i < n; ++i) {
        std::cout << scc[i] + 1 << ' ';
    }
    return 0;
}

