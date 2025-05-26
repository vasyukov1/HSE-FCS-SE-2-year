#include "include/StringSortTester.h"
#include "include/msd_radix_sort.h"
#include "include/string_merge_sort.h"
#include "include/ternary_quick_sort.h"
#include "include/quick_msd_radix_sort.h"
#include "include/TrackedString.h"
#include <iostream>

int counter = 0;

void quickSortWrapper(std::vector<std::string>& arr, int, int) {
    std::vector<TrackedString> tracked(arr.begin(), arr.end());
    std::sort(tracked.begin(), tracked.end());
}

void mergeSortWrapper(std::vector<std::string>& arr, int, int) {
    std::vector<TrackedString> tracked(arr.begin(), arr.end());
    std::stable_sort(tracked.begin(), tracked.end());
}

void teranryQuickSortWrapper(std::vector<std::string>& arr, int l, int) {
    ternaryQuickSort(arr, l);
}

void msdRadixSortWrapper(std::vector<std::string>& arr, int, int) {
    msdRadixSort(arr);
}

void quickMSDRadixSortWrapper(std::vector<std::string>& arr, int, int) {
    quickMSDRadixSort(arr);
}

int main() {
    try {
        StringSortTester tester("../data/strings.json");

        tester.testAlgorithm("quick_sort", quickSortWrapper);
        tester.testAlgorithm("merge_sort", mergeSortWrapper);

        tester.testAlgorithm("string_merge_sort", stringMergeSort);
        tester.testAlgorithm("ternary_quick_sort", teranryQuickSortWrapper);
        tester.testAlgorithm("msd_radix_sort", msdRadixSortWrapper);
        tester.testAlgorithm("quick_msd_radix_sort", quickMSDRadixSortWrapper);

        tester.saveResult("data/results.json");
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}