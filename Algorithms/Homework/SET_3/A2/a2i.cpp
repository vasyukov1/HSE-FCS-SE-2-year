#include <iostream>
#include <vector>
using std::cout;
using std::cin;
using std::vector;

void merge(vector<int>& array, int left, int mid, int right);
void mergeSort(vector<int>& array, int left, int right);
void insertionSort(vector<int>& array, int left, int right);
void hybridMergeSort(vector<int>& array, int left, int right, int threshold);

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr); 

    int n;
    cin >> n;

    vector<int> array;
    for (int i = 0; i < n; ++i) {
        int el;
        cin >> el;
        array.push_back(el);
    }

    int threshold = 15;
    hybridMergeSort(array, 0, n - 1, threshold);

    for (int i = 0; i < n; ++i) {
        cout << array[i] << ' ';
    }

    return 0;
}

void merge(vector<int>& array, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> array1(n1);
    vector<int> array2(n2);

    for (int i = 0; i < n1; ++i) {
        array1[i] = array[left + i];
    }
    for (int j = 0; j < n2; ++j) {
        array2[j] = array[mid + j + 1];
    }

    int i = 0;
    int j = 0;
    int k = left;
    while (i < n1 && j < n2) {
        if (array1[i] < array2[j]) {
            array[k++] = array1[i++];
        } else {
            array[k++] = array2[j++];
        }
    }
    while (i < n1) {
        array[k++] = array1[i++];
    }
    while (j < n2) {
        array[k++] = array2[j++];
    }
}

void mergeSort(vector<int>& array, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(array, left, mid);
        mergeSort(array, mid + 1, right);
        merge(array, left, mid, right);
    }
}

void insertionSort(vector<int>& array, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int el = array[i];
        int j = i - 1;
        while (el < array[j]) {
            array[j + 1] = array[j];
            --j;
        }
        array[j + 1] = el;
    }
}

void hybridMergeSort(vector<int>& array, int left, int right, int threshold) {
    if (right - left + 1 < threshold) {
        insertionSort(array, left, right);
        return;
    }

    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(array, left, mid);
        mergeSort(array, mid + 1, right);
        merge(array, left, mid, right);
    }
}