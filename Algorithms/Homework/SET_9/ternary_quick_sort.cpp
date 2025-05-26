#include <iostream>
#include <string>
#include <vector>
using std::cin;
using std::cout;
using std::string;
using std::vector;

vector<string> ternaryQuickSort(vector<string>& arr, size_t l) {
    if (arr.size() <= 1) {
        return arr;
    }

    vector<string> r;
    vector<string> r_excl;
    for (string& s : arr) {
        if (s.size() == l) {
            r_excl.push_back(s);
        } else {
            r.push_back(s);
        }
    }

    string pivot = r[r.size() / 2];

    vector<string> r_less;
    vector<string> r_equal;
    vector<string> r_great;
    for (string& s : r) {
        if (s[l] < pivot[l]) {
            r_less.push_back(s);
        } else if (s[l] == pivot[l]) {
            r_equal.push_back(s);
        } else {
            r_great.push_back(s);
        }
    }

    r_less = ternaryQuickSort(r_less, l);
    r_equal = ternaryQuickSort(r_equal, l + 1);
    r_great = ternaryQuickSort(r_great, l);

    arr = r_excl;
    arr.insert(arr.end(), r_less.begin(), r_less.end());
    arr.insert(arr.end(), r_equal.begin(), r_equal.end());
    arr.insert(arr.end(), r_great.begin(), r_great.end());
    return arr;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    cin >> n;
    vector<string> words(n);
    for (int i = 0; i < n; ++i) {
        cin >> words[i];
    }

    words = ternaryQuickSort(words, 0);
    for (string& s : words) {
        cout << s << '\n';
    }

    return 0;
}
