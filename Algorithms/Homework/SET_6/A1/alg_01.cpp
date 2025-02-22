#include <vector>
#include <algorithm>

struct Edge {
    int u;
    int v;
    int weight;

    bool operator<(const Edge& other) const {
        return weight > other.weight;
    }
};

bool isRelated(std::vector<Edge>& T, int V);

std::vector<Edge> ALG_1(std::vector<Edge> E, int V) {
    std::sort(E.begin(), E.end());
    std::vector<Edge> T = E;

    for (Edge& edge : T) {
        auto it = std::find(T.begin(), T.end(), edge);
        T.erase(it);
        if (!isRelated(T, V)) {
            T.insert(it, edge);
        }
    }

    return T;
}

