#include <iostream>
#include <vector>
#include <algorithm>
using std::cin;
using std::cout;
using std::vector;

int main() {
    int n, m;
    cin >> n >> m;

    vector w(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        cin >> w[i + 1];
    }

    vector c(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        cin >> c[i + 1];
    }

    vector t(n + 1, vector(m + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (w[i] <= j) {
                t[i][j] = std::max(t[i - 1][j], t[i - 1][j - w[i]] + c[i]);
            } else {
                t[i][j] = t[i - 1][j];
            }
        }
    }

    cout << t[n][m] << '\n';
    return 0;
}
