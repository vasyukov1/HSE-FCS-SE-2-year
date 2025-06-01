#include <iomanip>
#include <iostream>
#include <vector>
#include <queue>
using std::cin;
using std::cout;
using std::vector;

struct Item {
    double value;
    double weight;
};

struct Compare {
    bool operator()(const Item& lhs, const Item& rhs) const {
        return lhs.value / lhs.weight < rhs.value / rhs.weight;
    }
};

int main() {
    int n;
    double W;
    cin >> n >> W;

    std::priority_queue<Item, vector<Item>, Compare> value_per_weight;

    double value, weight;
    for (int i = 1; i < n + 1; ++i) {
        cin >> value >> weight;

        value_per_weight.push({value, weight});
    }

    double final_cost = 0;
    for (int i = 0; i < n; ++i) {
        auto [value, weight] = value_per_weight.top();
        value_per_weight.pop();
        if (weight <= W) {
            final_cost += value;
            W -= weight;
            if (W == 0) {
                break;
            }
        } else {
            final_cost += value * (W / weight);
            break;
        }
    }

    cout << std::fixed << std::setprecision(3) <<final_cost << '\n';
    return 0;
}
