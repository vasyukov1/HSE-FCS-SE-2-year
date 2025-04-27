#include <iostream>
#include <vector>
#include <string>

long long palindrome(const std::string& s) {
    const int n = s.size();
    long long count = 0;

    std::vector<long long> d1(n);
    long long l = 0;
    long long r = -1;
    for (long long i = 0; i < n; ++i) {
        int k = 1;
        if (i <= r) {
            k = std::min(d1[l + r - i], r - i);
        }

        while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) {
            ++k;
        }

        d1[i] = k;
        count += d1[i];

        if (i + k > r) {
            l = i - k;
            r = i + k;
        }
    }

    std::vector<long long> d2(n);
    l = 0;
    r = -1;
    for (long long i = 0; i < n; ++i) {
        long long k = 0;
        if (i <= r) {
            k = std::min(d2[l + r - i + 1], r - i + 1);
        }

        while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) {
            ++k;
        }

        d2[i] = k;
        count += d2[i];

        if (i + k - 1 > r) {
            l = i - k;
            r = i + k - 1;
        }
    }

    return count;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string w;
    std::getline(std::cin, w);
    std::cout << palindrome(w) << '\n';
    return 0;
}

