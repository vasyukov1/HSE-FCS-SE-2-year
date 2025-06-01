#include <iostream>
#include <vector>
#include <cmath>
using std::cin;
using std::cout;
using std::vector;

int main() {
    int l, n;
    cin >> l >> n;

    vector cuts(n + 2, 0);
    cuts[n + 1] = l;

    for (int i = 1; i <= n; ++i) {
        cin >> cuts[i];
    }

    vector dp(n + 2, vector(n + 2, 0));

    for (int len = 2; len <= n + 1; ++len) {
        for (int i = 0; i + len <= n + 1; ++i) {
            int j = i + len;
            dp[i][j] = 1e9;
            for (int cut = i + 1; cut < j; ++cut) {
                dp[i][j] = std::min(
                    dp[i][j],
                    dp[i][cut] + dp[cut][j] + cuts[j] - cuts[i]
                );
            }
        }
    }

    cout << dp[0][n + 1] << '\n';
    return 0;
}
