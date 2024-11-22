#include <iostream>
#include <vector>
#include <cmath>
using std::cin;
using std::cout;
using std::vector;

void radixSort(vector<int>& A);

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    cin >> n;

    int el;
    vector<int> A;
    for (int i = 0; i < n; ++i) {
        cin >> el;
        A.push_back(el);
    }

    radixSort(A);

    for (int i = 0; i < n; ++i) {
        cout << A[i] << ' ';
    }

    return 0;
}

void radixSort(vector<int>& A) {
    const int BASE = 256;
    const int BYTE_SIZE = 8;
    const int MAX_BITS = sizeof(int) * BYTE_SIZE;

    int max_digit = MAX_BITS / BYTE_SIZE;

    int min_num = INT_MAX;
    for (int i = 0; i < A.size(); ++i) {
        if (A[i] < min_num) {
            min_num = A[i];
        }
    }
    for (int i = 0; i < A.size(); ++i) {
        A[i] -= min_num;
    }

    vector<int> B(A.size());
    for (int digit = 0; digit < max_digit; ++digit) {
        vector<int> count(BASE, 0);

        for (int i = 0; i < A.size(); ++i) {
            int num = A[i];
            int byte = (num >> (digit * BYTE_SIZE)) & (BASE - 1);
            ++count[byte];
        }

        for (int i = 1; i < BASE; ++i) {
            count[i] += count[i - 1];
        }

        for (int i = A.size() - 1; i >= 0; --i) {
            int num = A[i];
            int byte = (num >> (digit * BYTE_SIZE)) & (BASE - 1);
            B[--count[byte]] = num;
        }

        A = B;
    }

    for (int i = 0; i < A.size(); ++i) {
        A[i] += min_num;
    }
}