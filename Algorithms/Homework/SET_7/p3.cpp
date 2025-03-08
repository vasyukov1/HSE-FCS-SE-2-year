#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

std::vector<std::vector<int>> graph;
std::unordered_map<std::string, int> nodes;
std::vector<std::vector<bool>> used;
int count = 0;

bool bfs(const int s, const int t, std::vector<int>& prev) {
    prev.assign(count, -1);
    prev[s] = -2;

    std::queue<int> q;
    q.push(s);

    while (!q.empty()) {
        const int u = q.front();
        q.pop();

        for (size_t i = 0; i < graph[u].size(); i++) {
            const int v = graph[u][i];
            if (prev[v] == -1 && !used[u][i]) {
                prev[v] = u;
                if (v == t) {
                    return true;
                }
                q.push(v);
            }
        }
    }
    return false;
}

int maxFlow(const int s, const int t) {
    int flow = 0;
    std::vector<int> prev(graph.size());

    for (int i = 0; i < count; i++) {
        used[i].assign(used[i].size(), false);
    }

    while (bfs(s, t, prev)) {
        int v = t;
        while (v != s) {
            const int u = prev[v];
            for (size_t i = 0; i < graph[u].size(); i++) {
                if (graph[u][i] == v) {
                    used[u][i] = true;
                    break;
                }
            }
            for (size_t i = 0; i < graph[v].size(); i++) {
                if (graph[v][i] == u) {
                    used[v][i] = true;
                    break;
                }
            }
            v = u;
        }
        ++flow;
    }
    return flow;
}

int getIndex(const std::string& person) {
    if (!nodes.contains(person)) {
        nodes[person] = count++;
        graph.emplace_back();
        used.emplace_back();
    }
    return nodes[person];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, k;
    std::cin >> n >> m >> k;

    graph.resize(n * 2);
    used.resize(n * 2);

    std::string u, v;
    for (int i = 0; i < m; ++i) {
        std::cin >> u >> v;
        int uIndex = getIndex(u);
        int vIndex = getIndex(v);
        used[uIndex].push_back(false);
        used[vIndex].push_back(false);
        graph[uIndex].push_back(vIndex);
        graph[vIndex].push_back(uIndex);
    }

    std::string s, t;
    for (int i = 0; i < k; ++i) {
        std::cin >> s >> t;
        if (!nodes.contains(s) || !nodes.contains(t)) {
            std::cout << "0\n";
            continue;
        }
        std::cout << maxFlow(nodes[s], nodes[t]) << '\n';
    }

    return 0;
}
