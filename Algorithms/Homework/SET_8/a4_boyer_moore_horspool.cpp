#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

std::unordered_map<char, int> distance(const std::string& s) {
    std::unordered_map<char, int> dist;

    const int m = static_cast<int>(s.size());

    for (int i = 0; i < m - 1; ++i) {
        dist[s[i]] = m - i - 1;
    }

    return dist;
}

void bmh(const std::string& w, const std::string& s) {
    const std::unordered_map<char, int> image = distance(w);
    std::vector<size_t> result;

    const int m = static_cast<int>(w.size());
    const int n = static_cast<int>(s.size());
    const int default_shift = m;

    int i = m - 1;
    while (i < n) {
        int k = i;
        int j = m - 1;

        while (j >= 0 && s[k] == w[j]) {
            --j;
            --k;
        }

        if (j < 0) {
            result.push_back(k + 1);
            ++i;
        } else {
            char c = s[i];
            i += image.contains(c) ? image.at(c) : default_shift;
        }
    }

    std::cout << result.size() << '\n';
    for (const size_t q : result) {
        std::cout << q << '\n';
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string w, s;
    std::cin >> w;
    std::cin >> s;

    if (w.empty() || s.empty() || w.size() > s.size()) {
        std::cout << 0 << '\n';
        return 0;
    }

    bmh(w, s);
    return 0;
}
