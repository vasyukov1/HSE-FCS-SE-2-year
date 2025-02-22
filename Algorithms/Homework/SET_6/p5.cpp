#include <iostream>
#include <vector>

class FloydWarshall {
private:
    std::vector<std::vector<long long>> dist;
    std::vector<std::vector<int>> path;
    int size;

public:
    FloydWarshall(int n) : size(n), dist(n, std::vector(n, LONG_LONG_MAX)), path(n, std::vector(n, -1)) {
        for (int i = 0; i < n; ++i) {
            dist[i][i] = 0;
            path[i][i] = i;
        }
    }

    void addEdge(int a, int b, long long c) {
        dist[a][b] = c;
        path[a][b] = a;
    }

    std::vector<std::vector<long long>> getDist() const {
        return dist;
    }

    void find() {
        for (int k = 0; k < size; ++k) {
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    if (dist[i][k] < LONG_LONG_MAX &&
                        dist[k][j] < LONG_LONG_MAX &&
                        dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        path[i][j] = path[k][j];
                        }
                }
            }
        }
    }

    void printDist() const {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (i != j) {
                    long long distance;
                    if (dist[i][j] < LONG_LONG_MAX) {
                        distance = dist[i][j] ;
                    } else {
                        distance = -1;
                    }
                    std::cout << i << ' ' << j << ' ' << distance << '\n';
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

    FloydWarshall floyd_warshall(n);
    int a, b, c;
    for (int i = 0; i < m; ++i) {
        std::cin >> a >> b >> c;
        floyd_warshall.addEdge(a, b, c);
    }

    floyd_warshall.find();
    floyd_warshall.printDist();
    return 0;
}