#include <iostream>
#include "Graph.h"

int main() {
    Graph g;

    /*
     *      0 - 1 - 3
     *       \ /
     *        2
     */

    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(1, 3);

    std::cout << "Eulerian Cycle: " << (g.hasEulerianCycle() ? "Yes" : "No") << "\n";
    std::cout << "Eulerian Path: " << (g.hasEulerianPath() ? "Yes" : "No") << "\n";

    return 0;
}
