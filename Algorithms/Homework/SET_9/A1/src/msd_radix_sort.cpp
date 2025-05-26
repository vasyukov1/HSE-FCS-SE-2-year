#include "../include/msd_radix_sort.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

const int ALPHABET_SIZE = 128;
const char PADDING_CHAR = 0;

void msdSort(vector<string>& arr, int left, int right, int pos, vector<string>& aux) {
    if (right - left <= 1) {
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
        msdSort(arr, l, r, pos + 1, aux);
    }
}

void msdRadixSort(vector<string>& arr) {
    vector<string> aux(arr.size());
    msdSort(arr, 0, arr.size(), 0, aux);
}