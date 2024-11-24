#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <chrono>
#include <fstream>
using std::cout;
using std::vector;
using std::string;

void merge(vector<int>& array, int left, int mid, int right);
void mergeSort(vector<int>& array, int left, int right);
void insertionSort(vector<int>& array, int left, int right);
void hybridMergeSort(vector<int>& array, int left, int right, int threshold);

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

    if (left - right) {
        int mid = left + (right - left) / 2;
        mergeSort(array, left, mid);
        mergeSort(array, mid + 1, right);
        merge(array, left, mid, right);
    }
}
