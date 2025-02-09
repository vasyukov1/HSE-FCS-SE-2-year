#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

struct Subarray {
    int start;
    int length;
};

struct Anagram {
    int length;
    int a;
    int b;
};

void find(int* a, const int a_size, int* b, const int b_size, Anagram* max_anagram, int& max_size) {
    constexpr Anagram bad_anagram = {0, -1, -1};
    const int n = std::min(a_size, b_size);
    if (n == 0 || n < max_size) {
        *max_anagram = bad_anagram;
        return;
    }

    if (a_size == 1) {
        for (int i = 0; i < b_size; ++i) {
            if (*a == b[i]) {
                if (max_anagram->length < a_size) {
                    max_size = std::max(max_size, 1);
                    *max_anagram = {1, 1, i + 1};
                    return;
                }
            }
        }
        *max_anagram = bad_anagram;
        return;
    }
    if (b_size == 1) {
        for (int i = 0; i < a_size; ++i) {
            if (*b == a[i]) {
                if (max_anagram->length < b_size) {
                    max_size = std::max(max_size, 1);
                    *max_anagram = {1, i + 1, 1};
                    return;
                }
            }
        }
        *max_anagram = bad_anagram;
        return;
    }

    int a_xor = 0;
    int b_xor = 0;
    for (int i = 0; i < n; ++i) {
        a_xor ^= a[i];
        b_xor ^= b[i];
    }

    if (a_xor == b_xor) {
        std::vector a_sorted(a, a + n);
        std::vector b_sorted(b, b + n);
        std::ranges::sort(a_sorted.begin(), a_sorted.end());
        std::ranges::sort(b_sorted.begin(), b_sorted.end());

        if (a_sorted == b_sorted) {
            max_size = std::max(max_size, n);
            *max_anagram = {n, 1, 1};
            return;
        }
    }

    const int count = 4;
    Anagram sub[count];
    find(a, a_size, b, b_size - 1, &sub[0], max_size);
    find(a, a_size, b + 1, b_size - 1, &sub[1], max_size);
    if (sub[1].length > 0) {
        ++sub[1].b;
    }

    find(a, a_size - 1, b, b_size, &sub[2], max_size);
    find(a + 1, a_size - 1, b, b_size, &sub[3], max_size);
    if (sub[3].length > 0) {
        ++sub[3].a;
    }

    *max_anagram = *std::max_element(sub, sub + count, [](const Anagram& lhs, const Anagram& rhs) {
        return lhs.length < rhs.length;
    });

    max_size = std::max(max_size, max_anagram->length);
}

void searcher(int* a, int a_size, int* b, int b_size, Anagram* max_anagram) {
    int max_size = 0;
    *max_anagram = {0, -1, -1};
    std::unordered_set a_elements(a, a + a_size);
    std::unordered_set b_elements(b, b + b_size);

    std::vector<Subarray> segments;

    int b_start = 0;
    for (int i = 0; i <= b_size; ++i) {
        if (!a_elements.contains(b[i]) || i == b_size) {
            if (i > b_start) {
                segments.push_back({ b_start, i - b_start});
            }
            b_start = i + 1;
        }
    }

    int a_start = 0;
    for (int i = 0; i <= a_size; ++i) {
        if (!b_elements.contains(a[i]) || i == a_size) {
            const int size = i - a_start;
            for (const Subarray& segment : segments) {
                Anagram anagram;
                find(a + a_start, size, b + segment.start, segment.length, &anagram, max_size);
                if (anagram.length > max_anagram->length) {
                    *max_anagram = anagram;
                    max_anagram->a += a_start;
                    max_anagram->b += segment.start;
                }
            }
            a_start = i + 1;
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (int& el : a) {
        std::cin >> el;
    }

    int m;
    std::cin >> m;
    std::vector<int> b(m);
    for (int& el : b) {
        std::cin >> el;
    }

    Anagram result;
    searcher(a.data(), n, b.data(), m, &result);
    std::cout << result.length << " " << result.a << " " << result.b << '\n';
    return 0;
}