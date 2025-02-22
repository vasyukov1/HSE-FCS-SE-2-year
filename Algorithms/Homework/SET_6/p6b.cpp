#include <iostream>
#include <queue>
#include <vector>

const int INF = std::numeric_limits<int>::max();

struct State {
    int year;
    int cost;
    int steps;

    bool operator>(const State& other) const {
        if (cost != other.cost) {
            return cost > other.cost;
        }
        return steps > other.steps;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, a, b;
    std::cin >> n >> a >> b;

    std::vector cost(n + 1, INF);
    std::vector steps(n + 1, INF);
    std::vector<std::pair<int, std::string>> prev(n + 1, {-1, ""});
    std::priority_queue<State, std::vector<State>, std::greater<State>> queue;

    cost[a] = 0;
    steps[a] = 0;
    queue.push({a, 0, 0});

    std::vector year_costs(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        int year = i;
        int sum = 0;
        while (year) {
            sum += year % 10;
            year /= 10;
        }
        year_costs[i] = sum;
    }

    while (!queue.empty()) {
        State state = queue.top();
        queue.pop();

        if (state.cost != cost[state.year] || state.steps != steps[state.year]) {
            continue;
        }

        std::string str_state_year = std::to_string(state.year);
        int len_str_year = str_state_year.size();

        for (int i = 0; i < len_str_year; ++i) {
            if (str_state_year[i] == '0') {
                continue;
            }
            for (int j = i; j < len_str_year; ++j) {
                std::string sub_year = str_state_year.substr(i, j - i + 1);
                int year = std::stoi(sub_year);

                int new_cost = state.cost + year_costs[year];
                int new_steps = state.steps + 1;

                for (const int sign : {-1, 1}) {
                    const int new_year = state.year + sign * year;
                    if (new_year < 1 || new_year > n) {
                        continue;
                    }

                    if (new_cost < cost[new_year] || (new_cost == cost[new_year] && new_steps < steps[new_year])){
                        queue.push({new_year, new_cost, new_steps});
                        cost[new_year] = new_cost;
                        steps[new_year] = new_steps;
                        prev[new_year] = {state.year, (sign == 1 ? '+' : '-') + sub_year};
                    }
                }
            }
        }
    }

    if (cost[b] == INF) {
        std::cout << -1;
        return 0;
    }

    std::vector<std::string> path;
    int vertex = b;
    while (vertex != a) {
        std::pair<int, std::string> year = prev[vertex];
        path.push_back(year.second);
        vertex = year.first;
    }
    std::reverse(path.begin(), path.end());

    std::cout << cost[b] << '\n';
    std::cout << steps[b] << '\n';
    for (std::string& action : path) {
        std::cout << action << "\n";
    }
    return 0;
}