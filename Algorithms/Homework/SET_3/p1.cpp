#include <iostream>
#include <vector>
using std::cin;
using std::cout;
using std::vector;

void heapify(vector<int>& A, int n, int i);
void buildMaxHeap(vector<int>& A);
void heapSort(vector<int>& A);

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

    heapSort(A);

    for (int i = 0; i < A.size(); ++i) {
        cout << A[i] << ' ';
    }

    return 0;
}

void heapify(vector<int>& A, int n, int i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int max_el = i;

    if (left < n && A[left] > A[max_el]) {
        max_el = left;
    }
    if (right < n && A[right] > A[max_el]) {
        max_el = right;
    }
    if (max_el != i) {
        std::swap(A[max_el], A[i]);
        heapify(A, n, max_el);
    }
}

void buildMaxHeap(vector<int>& A) {
    int n = A.size();
    for (int i = n / 2; i >= 0; --i) {
        heapify(A, n, i);
    }
}

void heapSort(vector<int>& A) {
    int n = A.size();
    buildMaxHeap(A);
    for (int i = n; i > 0; --i) {
        std::swap(A[0], A[i - 1]);
        heapify(A, i - 1, 0);
    }
}