#include <vector>

struct Edge {
    int u;
    int v;
    int weight;
};

bool isCycle( std::vector<Edge> T);

Edge findMaxEdgeInCycle(std::vector<Edge> T);

std::vector<Edge> ALG_2(std::vector<Edge> E, int V) {
    std::vector<Edge> T;
    for (Edge& edge : E) {
        T.push_back(edge);
        if (!isCycle(T)) {
            Edge e_max = findMaxEdgeInCycle(T);
            auto it = std::find(T.begin(), T.end(), e_max);
            T.erase(it);
        }
    }
    return T;
}
