#include <algorithm>
#include <iostream>
#include <vector>

bool isContains(const std::vector<std::string>& a, const std::vector<std::string>& b) {
    int i = 0;
    int j = 0;
    while (i < a.size() && j < b.size()) {
        if (a[i] == b[j]) {
            ++i;
            ++j;
        } else if (a[i] > b[j]) {
            ++j;
        } else {
            return false;
        }
    }
    return i == a.size();
}

bool dfs(const int u, std::vector<std::vector<int>>& graph, std::vector<bool>& used, std::vector<int>& match) {
    for (const int v : graph[u]) {
        if (!used[v]) {
            used[v] = true;
            if (match[v] == -1 || dfs(match[v], graph, used, match)) {
                match[v] = u;
                return true;
            }
        }
    }
    return false;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<std::vector<std::string>> juices(n);
    std::vector sizes(n, 0);

    int k;
    std::string fruit;
    for (int i = 0; i < n; ++i) {
        std::cin >> k;
        sizes[i] = k;
        for (int j = 0; j < k; ++j) {
            std::cin >> fruit;
            juices[i].push_back(fruit);
        }
        sort(juices[i].begin(), juices[i].end());
    }

    std::vector<std::vector<int>> graph(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j || sizes[i] > sizes[j] || (sizes[i] == sizes[j] && i >= j)) {
                continue;
            }
            if (isContains(juices[i], juices[j])) {
                graph[i].push_back(j);
            }
        }
    }

    std::vector match(n, -1);
    int count = 0;

    for (int i = 0; i < n; ++i) {
        std::vector used(n, false);
        if (dfs(i, graph, used, match)) {
            ++count;
        }
    }

    std::cout << n - count << '\n';
    return 0;
}
