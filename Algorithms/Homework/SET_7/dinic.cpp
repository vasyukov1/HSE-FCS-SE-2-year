#include <iostream>
#include <vector>
#include <queue>

class Dinic {
private:
    struct Edge {
        int to;
        int reverse;
        long long capacity;
    };

    int n;
    std::vector<std::vector<Edge>> graph;
    std::vector<int> level;
    std::vector<int> ptr;

    long long dfs(const int s, const int t, const long long flow) {
        if (s == t) {
            return flow;
        }

        for (auto& i = ptr[s]; i < graph[s].size(); ++i) {
            Edge& e = graph[s][i];
            if (level[e.to] == level[s] + 1 && e.capacity > 0) {
                const long long min_flow = std::min(flow, e.capacity);
                const long long pushed = dfs(e.to, t, min_flow);
                if (pushed > 0) {
                    e.capacity -= pushed;
                    graph[e.to][e.reverse].capacity += pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    bool bfs(const int s, const int t) {
        level.assign(n, -1);
        level[s] = 0;
        std::queue<int> q;
        q.push(s);
        while (!q.empty()) {
            const int v = q.front();
            q.pop();
            for (const Edge& e : graph[v]) {
                if (level[e.to] == -1 && e.capacity > 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

public:
    Dinic(int n) : n(n), graph(n) {}

    void addEdge(const int from, const int to, const int cap) {
        graph[from].push_back({to, static_cast<int>(graph[to].size()), cap});
        graph[to].push_back({from, static_cast<int>(graph[from].size() - 1), 0});
    }

    long long maxFlow(const int s, const int t) {
        long long res = 0;
        while(bfs(s, t)) {
            ptr.assign(n, 0);
            while (const long long pushed = dfs(s, t, LONG_LONG_MAX)) {
                res += pushed;
            }
        }
        return res;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    int m;
    std::cin >> n >> m;

    Dinic dinic(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b, c;
        std::cin >> a >> b >> c;
        dinic.addEdge(a, b, c);
    }

    const long long maxFlow = dinic.maxFlow(1, n);
    std::cout << maxFlow;
    return 0;
}
