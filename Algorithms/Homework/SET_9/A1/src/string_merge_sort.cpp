#include "../include/string_merge_sort.h"
#include <chrono>
#include <string>
#include <vector>
using std::string;
using std::vector;

void mergeLCP(vector<string>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<string> L(n1);
    vector<string> R(n2);

    for (int i = 0; i < n1; ++i) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; ++j) {
        R[j] = arr[mid + j + 1];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        int startLCP = 0;
        size_t n = std::min(L[i].size(), R[j].size());

        while (startLCP < n && L[i][startLCP] == R[j][startLCP]) {
            ++counter;
            ++startLCP;
        }

        if (startLCP == n) {
            if (L[i].size() < R[j].size()) {
                arr[k++] = L[i++];
            } else {
                arr[k++] = R[j++];
            }
        } else {
            if (L[i][startLCP] < R[j][startLCP]) {
                arr[k++] = L[i++];
            } else {
                arr[k++] = R[j++];
            }
            ++counter;
        }
    }
    while (i < n1) {
        arr[k++] = L[i++];
    }
    while (j < n2) {
        arr[k++] = R[j++];
    }
}

void stringMergeSort(vector<string>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        stringMergeSort(arr, left, mid);
        stringMergeSort(arr, mid + 1, right);
        mergeLCP(arr, left, mid, right);
    }
}