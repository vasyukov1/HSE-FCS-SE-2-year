#include <iostream>
#include <vector>

std::vector<int> prefix(const std::string& s) {
    const size_t length = s.size();
    std::vector p(length, 0);

    for (size_t i = 1; i < length; ++i) {
        int k = p[i - 1];

        while (k > 0 && s[k] != s[i]) {
            k = p[k - 1];
        }
        if (s[k] == s[i]) {
            ++k;
        }
        p[i] = k;
    }

    return p;
}

int main() {
    std::string s;
    std::cin >> s;
    std::vector<int> p = prefix(s);
    for (const auto k: p) {
        std::cout << k << ' ';
    }
    std::cout << '\n';
    return 0;
}
