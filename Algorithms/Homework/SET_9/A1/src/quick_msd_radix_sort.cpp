#include "../include/quick_msd_radix_sort.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

const int ALPHABET_SIZE = 128;
const char PADDING_CHAR = 0;
const int SWITCH = 74;

void quickSort(vector<string>& arr, size_t l) {
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

    quickSort(r_less, l);
    quickSort(r_equal, l + 1);
    quickSort(r_great, l);

    arr = r_excl;
    arr.insert(arr.end(), r_less.begin(), r_less.end());
    arr.insert(arr.end(), r_equal.begin(), r_equal.end());
    arr.insert(arr.end(), r_great.begin(), r_great.end());
}

void msdSort_quick(vector<string>& arr, int left, int right, int pos, vector<string>& aux) {
    if (right - left < SWITCH) {
        vector sub(arr.begin() + left, arr.begin() + right);
        quickSort(sub, pos);
        for (int i = left; i < right; ++i) {
            arr[i] = sub[i - left];
        }
        return;
    }
    vector count(ALPHABET_SIZE + 2, 0);

    for (int i = left; i < right; ++i) {
        char c = pos < arr[i].size() ? arr[i][pos] : PADDING_CHAR;
        count[c + 1]++;
    }

    for (int i = 0; i < ALPHABET_SIZE + 1; ++i) {
        count[i + 1] += count[i];
    }

    for (int i = left; i < right; ++i) {
        char c = pos < arr[i].size() ? arr[i][pos] : PADDING_CHAR;
        aux[count[c]++] = arr[i];
    }

    for (int i = left; i < right; ++i) {
        arr[i] = aux[i - left];
    }

    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        int l = left + count[i];
        int r = left + count[i + 1];
        msdSort_quick(arr, l, r, pos + 1, aux);
    }
}

void quickMSDRadixSort(vector<string>& arr) {
    vector<string> aux(arr.size());
    msdSort_quick(arr, 0, arr.size(), 0, aux);
}
