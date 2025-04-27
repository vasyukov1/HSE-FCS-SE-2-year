#include <iostream>
#include <vector>
#include <string>

int levenstein(const std::string& x, const std::string& y) {
    int n = static_cast<int>(x.size());
    int m = static_cast<int>(y.size());

    std::vector dp(n + 1, std::vector<int>(m + 1));

    for (int i = 0; i <= n; ++i) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= m; ++j) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (x[i - 1] == y[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = std::min(dp[i - 1][j], std::min(dp[i][j - 1], dp[i - 1][j - 1])) + 1;
            }
        }
    }

    return dp[n][m];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int k;
    std::cin >> k;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string x, y;
    for (int i = 0; i < k; ++i) {
        std::getline(std::cin, x);
        std::getline(std::cin, y);
        std::cout << levenstein(x, y) << ' ';
    }
    return 0;
}
