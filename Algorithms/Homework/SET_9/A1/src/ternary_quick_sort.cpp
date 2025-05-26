#include "../include/ternary_quick_sort.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

void ternaryQuickSort(vector<string>& arr, size_t l) {
    if (arr.size() <= 1) {
        return;
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
        ++counter;
        if (s[l] < pivot[l]) {
            r_less.push_back(s);
        } else {
            ++counter;
            if (s[l] == pivot[l]) {
                r_equal.push_back(s);
            } else {
                r_great.push_back(s);
            }
        }
    }

    ternaryQuickSort(r_less, l);
    ternaryQuickSort(r_equal, l + 1);
    ternaryQuickSort(r_great, l);

    arr = r_excl;
    arr.insert(arr.end(), r_less.begin(), r_less.end());
    arr.insert(arr.end(), r_equal.begin(), r_equal.end());
    arr.insert(arr.end(), r_great.begin(), r_great.end());
}