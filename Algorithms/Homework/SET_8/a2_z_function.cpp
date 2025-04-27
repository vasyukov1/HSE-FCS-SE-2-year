#include <iostream>
#include <vector>
#include <string>

std::vector<int> z_function(const std::string& s) {
    std::vector p(s.size(), 0);
    int n = static_cast<int>(s.size());
    int start = 0;
    int end = 0;

    p[0] = s.size();

    for (int i = 1; i < n; ++i) {

        p[i] = std::max(0, std::min(end - i, p[i - start]));
        while (i + p[i] < n && s[p[i]] == s[i + p[i]]) {
            ++p[i];
        }

        if (i + p[i] > end) {
            start = i;
            end = i + p[i];
        }
    }
    return p;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string w;
    std::cin >> w;

    int n = w.size();
    std::vector<int> z = z_function(w);

    int prev = 0;
    for (int i = 1; i < n; ++i) {
        // std::cout << z[i] << " ";
        if (z[i] != 0) {
            std::cout << i;
            return 0;
        }
        prev = z[i];
    }
    if (prev == 0) {
        std::cout << n;
    }

    // for (int i = 1; i < n; ++i) {
    //     if (n % i == 0 || z[i] == n - i) {
    //         std::cout << i << '\n';
    //         return 0;
    //     }
    // }
    // std::cout << n << '\n';
    return 0;
}

