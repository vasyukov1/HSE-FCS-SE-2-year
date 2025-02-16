#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>

class Graph {
private:
    std::unordered_map<int, std::vector<int>> edges;
    int size = 0;

    void dfs(int v, std::unordered_set<int>& visited) const;
    bool isRelated() const;

public:
    void addEdge(int u, int v);
    bool hasEulerianCycle() const;
    bool hasEulerianPath() const;
};

#endif //GRAPH_H
