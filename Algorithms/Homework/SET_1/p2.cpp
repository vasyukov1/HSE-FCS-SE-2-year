#include <iostream>
#include <stack>
#include <vector>
using std::cin;
using std::cout;
using std::vector;
using std::stack;
using std::string;

int main() {
    int n;
    cin >> n;

    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    stack<int> train;
    for (int i = 0; i < n; ++i) {
        train.push(a[n - i - 1]);
    }

    stack<int> depot;
    string result;

    int need = 1;
    int way1 = 0;
    int way2 = 0;
    int ok = 0;
    while (ok != n) {
        if (!depot.empty() && depot.top() == need) {
            depot.pop();
            ++way2;
            ++need;
            if (way1 != 0) {
                result += "1 " + std::to_string(way1) + '\n';
                way1 = 0;
            }
        } else if (!depot.empty() && depot.top() != need && !train.empty()) {
            depot.push(train.top());
            train.pop();
            ++way1;
            ++ok;
            if (way2 != 0) {
                result += "2 " + std::to_string(way2) + '\n';
                way2 = 0;
            }
        } else if (depot.empty() && !train.empty()) {
            depot.push(train.top());
            train.pop();
            ++way1;
            if (way2 != 0) {
                result += "2 " + std::to_string(way2) + '\n';
                way2 = 0;
            }
        }
        if (train.empty() && !depot.empty() && depot.top() != need) {
            cout << "0";
            return 0;
        }
        if (train.empty() && depot.empty()) {
            result += "2 " + std::to_string(way2) + '\n';
            cout << result;
            return 0;
        }
    }
    cout << result;
    return 0;
}