#include <iostream>
#include <string>
#include <vector>
using std::cin;
using std::cout;
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
        }
    }
    while (i < n1) {
        arr[k++] = L[i++];
    }
    while (j < n2) {
        arr[k++] = R[j++];
    }
}

void mergeSort(vector<string>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        mergeLCP(arr, left, mid, right);
    }
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

    mergeSort(words, 0, n - 1);
    for (string& s : words) {
        cout << s << '\n';
    }

    return 0;
}
