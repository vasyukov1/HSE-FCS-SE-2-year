#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>
#include <chrono>
#include <fstream>
#include <algorithm>
using std::cout;
using std::vector;
using std::string;

void insertionSort(vector<int>& array, int left, int right);
void heapify(vector<int>& A, int n, int i);
void buildMaxHeap(vector<int>& A);
void heapSort(vector<int>& A);
int partition(vector<int>& arr, int low, int high);
void quickSort(vector<int>& arr, int low, int high);
void introsort(vector<int>& arr, int low, int high);


class ArrayGenerator {
private:
    vector<int> baseArray;
    int maxLength;
    int minRange;
    int maxRange;
    

    void generateBaseArray() {
        std::mt19937 gen(static_cast<unsigned>(std::time(nullptr)));
        std::uniform_int_distribution<int> dist(minRange, maxRange);

        for (int i = 0; i <= maxLength; ++i) {
            baseArray.push_back(dist(gen));
        }
    }

public:
    ArrayGenerator() : maxLength(10000), minRange(0), maxRange(6000) {
        generateBaseArray();
    }

    vector<int> getRandomArray(int size) {
        if (size > maxLength) {
            size = maxLength;
        }
        return vector<int> (baseArray.begin(), baseArray.begin() + size);
    }

    vector<int> getReversedArray(int size) {
        if (size > maxLength) {
            size = maxLength;
        }
        vector<int> reversedArray(baseArray.begin(), baseArray.begin() + size);
        std::sort(reversedArray.rbegin(), reversedArray.rend());
        return reversedArray;
    }

    vector<int> getNearlySortedArray(int size, int swaps) {
        if (size > maxLength) {
            size = maxLength;
        }
        vector<int> nearlySortedArray(baseArray.begin(), baseArray.begin() + size);
        std::sort(nearlySortedArray.begin(), nearlySortedArray.end());

        std::mt19937 gen(static_cast<unsigned>(std::time(nullptr)));
        std::uniform_int_distribution<int> dist(0, size - 1);

        for (int i = 0; i < swaps; ++i) {
            std::swap(nearlySortedArray[dist(gen)], nearlySortedArray[dist(gen)]);
        }

        return nearlySortedArray;
    }
};

class SortTester {
public:
    double measureMergeSortTime(vector<int>& array, int size) {
        const int numTrials = 10;
        long long totalTime = 0;

        for (int trial = 0; trial < numTrials; ++trial) {
            vector<int> arrayToSort = array;
            auto start = std::chrono::high_resolution_clock::now();
            mergeSort(arrayToSort, 0, arrayToSort.size() - 1);
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            long long msec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
            totalTime += std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        }
            
        return static_cast<double> (totalTime) / numTrials;
    }

    double measureHybridMergeSortTime(vector<int>& array, int size, int threshold) {
        const int numTrials = 10;
        long long totalTime = 0;

        for (int trial = 0; trial < numTrials; ++trial) {
            vector<int> arrayToSort = array;
            auto start = std::chrono::high_resolution_clock::now();
            hybridMergeSort(arrayToSort, 0, arrayToSort.size() - 1, threshold);
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            long long msec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
            totalTime += std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        }
            
        return static_cast<double> (totalTime) / numTrials;
    }
};

int main() {
    ArrayGenerator generator;
    SortTester st;

    string fileName1 = "merge_sort_random.txt";
    string fileName2 = "merge_sort_reversed.txt";
    string fileName3 = "merge_sort_nearly_sorted.txt";

    string fileName4 = "hybrid_sort_random.txt";
    string fileName5 = "hybrid_sort_reversed.txt";
    string fileName6 = "hybrid_sort_nearly_sorted.txt";

    std::ofstream outFile1(fileName1);
    std::ofstream outFile2(fileName2);
    std::ofstream outFile3(fileName3);

    std::ofstream outFile4(fileName4);
    std::ofstream outFile5(fileName5);
    std::ofstream outFile6(fileName6);

    for (int size = 500; size <= 10000; size += 100) {
        vector<int> randomArray = generator.getRandomArray(size);
        vector<int> reversedArray = generator.getReversedArray(size);
        vector<int> nearlySortedArray = generator.getNearlySortedArray(size, size / 20);

        double randomTime = st.measureMergeSortTime(randomArray, size);
        double reversedTime = st.measureMergeSortTime(reversedArray, size);
        double nearlySortedTime = st.measureMergeSortTime(nearlySortedArray, size);

        outFile1 << size << ' ' << randomTime << '\n';
        outFile2 << size << ' ' << reversedTime << '\n';
        outFile3 << size << ' ' << nearlySortedTime << '\n';

        vector<int> threshold;
        threshold.push_back(5);
        threshold.push_back(10);
        threshold.push_back(20);
        threshold.push_back(30);
        threshold.push_back(50);

        outFile4 << size;
        outFile5 << size;
        outFile6 << size;
        for (int i = 0; i < threshold.size(); ++i) {
            randomTime = st.measureHybridMergeSortTime(randomArray, size, threshold[i]);
            reversedTime = st.measureHybridMergeSortTime(reversedArray, size, threshold[i]);
            nearlySortedTime = st.measureHybridMergeSortTime(nearlySortedArray, size, threshold[i]);
            
            outFile4 << ' ' << randomTime;
            outFile5 << ' ' << reversedTime;
            outFile6 << ' ' << nearlySortedTime;
        }
        outFile4 << '\n';
        outFile5 << '\n';
        outFile6 << '\n';
    }

    outFile1.close();
    outFile2.close();
    outFile3.close();
    outFile4.close();
    outFile5.close();
    outFile6.close();

    cout << "DONE!" << '\n';
    return 0;
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

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(std::vector<int>& arr, int low, int high, int depthLimit) {
    while (low < high) {
        int n = high - low + 1;

        // Переключение на Insertion Sort для небольших массивов
        if (n < 16) {
            insertionSort(arr, low, high);
            break;
        }

        // Переключение на Heap Sort при превышении глубинного лимита
        if (depthLimit == 0) {
            heapSort(arr, low, high);
            break;
        }

        int pivotIndex = partition(arr, low, high);

        if (pivotIndex - low < high - pivotIndex) {
            quickSort(arr, low, pivotIndex - 1, depthLimit - 1);
            low = pivotIndex + 1;
        } else {
            quickSort(arr, pivotIndex + 1, high, depthLimit - 1);
            high = pivotIndex - 1;
        }
    }
}

void introsort(vector<int>& arr, int low, int high) {
    int n = high - low + 1;
    int depthLimit = 2 * std::log2(n);

    quickSort(arr, low, high, depthLimit);
}