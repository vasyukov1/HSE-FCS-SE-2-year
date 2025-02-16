#include "Graph.h"

void Graph::dfs(int v, std::unordered_set<int>& visited) const {
    visited.insert(v);
    for (int u : edges.at(v)) {
        if (!visited.contains(u)) {
            dfs(u, visited);
        }
    }
}

bool Graph::isRelated() const {
    int start = -1;
    for (const auto& [v, neighbors] : edges) {
        if (!neighbors.empty()) {
            start = v;
            break;
        }
    }
    if (start == -1) {
        return true;
    }

    std::unordered_set<int> visited;
    dfs(start, visited);

    for (const auto& [v, neighbors] : edges) {
        if (!neighbors.empty() && !visited.contains(v)) {
            return false;
        }
    }
    return true;
}

void Graph::addEdge(int u, int v) {
    edges[u].push_back(v);
    edges[v].push_back(u);
    ++size;
}

bool Graph::hasEulerianCycle() const {
    if (!isRelated()) {
        return false;
    }

    for (const auto& [v, neighbors] : edges) {
        if (neighbors.size() % 2 != 0) {
            return false;
        }
    }
    return true;
}

bool Graph::hasEulerianPath() const {
    if (!isRelated()) {
        return false;
    }

    int oddCount = 0;
    for (const auto& [v, neighbors] : edges) {
        if (neighbors.size() % 2 != 0) {
           ++oddCount;
        }
    }
    return oddCount == 0 || oddCount == 2;
}