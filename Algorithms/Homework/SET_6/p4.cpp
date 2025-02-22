#include <iostream>
#include <vector>
#include <limits>

class BellmanFord {
private:
    struct Edge {
        int from;
        int to;
        int weight;
    };
    std::vector<Edge> edges;
    std::vector<int> dist;
    std::vector<bool> inf;
    int vertexNumber;

public:
    BellmanFord(int n, int m) : vertexNumber(n), dist(n, std::numeric_limits<int>::max()), inf(n , false) {
        dist[0] = 0;
        edges.reserve(m);
    }

    std::vector<int> getDist() const {
        return dist;
    }

    std::vector<bool> getInf() const {
        return inf;
    }

    void addEdge(int a, int b, int c) {
        edges.push_back({a, b, c});
    }

    void find() {
        for (int i = 0; i < vertexNumber - 1; ++i) {
            bool updated = false;
            for (Edge& edge : edges) {
                if (dist[edge.from] != std::numeric_limits<int>::max() &&
                    dist[edge.from] + edge.weight < dist[edge.to]) {
                    dist[edge.to] = dist[edge.from] + edge.weight;
                    updated = true;
                }
            }
            if (!updated) {
                break;
            }
        }

        for (int i = 0; i < vertexNumber; ++i) {
            for (Edge& edge : edges) {
                if (dist[edge.from] != std::numeric_limits<int>::max() &&
                    dist[edge.from] + edge.weight < dist[edge.to]) {
                    inf[edge.to] = true;
                    }
                if (inf[edge.from]) {
                    inf[edge.to] = true;
                }
            }
        }
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    BellmanFord bellman_ford(n, m);
    int a, b, c;
    for (int i = 0; i < m; ++i) {
        std::cin >> a >> b >> c;
        bellman_ford.addEdge(a, b, c);
    }

    bellman_ford.find();
    std::vector<int> dist = bellman_ford.getDist();
    std::vector<bool> inf = bellman_ford.getInf();
    for (int i = 1; i < n; ++i) {
        std::cout << (inf[i] ? "-inf" : std::to_string(dist[i])) << '\n';
    }

    return 0;
}