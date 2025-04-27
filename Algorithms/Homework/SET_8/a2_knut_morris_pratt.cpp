#include <iostream>
#include <vector>
#include <string>

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

void kmp(const std::string& w, const std::string& s) {
    const std::vector p = prefix(w);
    const size_t w_length = w.size();
    std::vector<size_t> result;

    int k = 0;
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == w[k]) {
            ++k;
            if (k == w_length) {
                const size_t start = i - w_length + 1;
                result.push_back(start);
                if (k > 0) {
                    k = p[k - 1];
                }
            }
            continue;
        }
        if (k > 0) {
            k = p[k - 1];
            --i;
        }
    }

    std::cout << result.size() << '\n';
    for (const size_t i : result) {
        std::cout << i << '\n';
    }
}

int main() {
    std::string w, s;
    std::cin >> w;
    std::cin >> s;

    kmp(w, s);
    return 0;
}
