#include <algorithm>
#include <iostream>
#include <vector>
using std::cin;
using std::cout;
using std::vector;
using std::pair;

struct Interval {
    int left;
    int right;

    Interval(int a, int b) : left(a), right(b) {}

    [[nodiscard]] std::size_t length() const {
        return right - left + 1;
    }

    [[nodiscard]] Interval overlap(const Interval& other) const {
        if (this->left <= other.right && other.left <= this->right) {
            const int overlap_left = std::max(this->left, other.left);
            const int overlap_right = std::min(this->right, other.right);
            return Interval{overlap_left, overlap_right};
        }
        return Interval{1, 0};
    }

    bool operator<(const Interval& other) const {
        return this->left < other.left || (this->left == other.left && this->right < other.right);
    }
};

void merge(int left, int right);
void mergeSort(int left, int right);
pair<Interval, int> findMaxOverlap(int left, int right);

vector<Interval> intervals;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    cin >> n;
    intervals.reserve(n);

    for (int i = 0; i < n; ++i){
        int a, b;
        cin >> a >> b;
        intervals.emplace_back(a, b);
    }

    mergeSort(0, n);
    Interval ans = findMaxOverlap(0, n).first;

    cout << ans.length() << '\n';
    if (ans.length() > 0) {
        cout << ans.left << ' ' << ans.right << '\n';
    }
}

void merge(int left, int right) {
    int mid = (left + right) / 2;
    int n1 = mid - left;
    int n2 = right - mid;

    vector<Interval> intervals1(n1, Interval{1, 0}), intervals2(n2, Interval{1, 0});
    for (int i = 0; i < n1; ++i) {
        intervals1[i] = intervals[left + i];
    }
    for (int i = 0; i < n2; ++i) {
        intervals2[i] = intervals[mid + i];
    }

    int i = 0;                  // Pointer for intervals1
    int j = 0;                  // Pointer for intervals2
    int k = left;               // Pointer for intervals
    while (i < n1 && j < n2) {
        if (intervals1[i] < intervals2[j]) {
            intervals[k++] = intervals1[i++];
        } else {
            intervals[k++] = intervals2[j++];
        }
    }
    while (i < n1) {
        intervals[k++] = intervals1[i++];
    }
    while (j < n2) {
        intervals[k++] = intervals2[j++];
    }
}

void mergeSort(int left, int right) {
    if (left + 1 >= right) {
        return;
    }
    int mid = (left + right) / 2;
    mergeSort(left, mid);
    mergeSort(mid, right);
    merge(left, right);
}

pair<Interval, int> findMaxOverlap(int left, int right) {
    Interval ans = {1, 0};

    if (left + 1 >= right) {
        if (left == right) {
            return std::make_pair(Interval{1, 0}, INT_MIN);
        }
        return std::make_pair(Interval{1, 0}, intervals[left].right);
    }

    int mid = (left + right) / 2;
    pair<Interval, int>  find_right = findMaxOverlap(mid, right);
    pair<Interval, int>  find_left = findMaxOverlap(left, mid);

    Interval center = {intervals[mid].left, find_left.second};

    for (int i = mid; i < right; ++i){
        Interval center_overlap = intervals[i].overlap(center);
        if (ans.length() < center_overlap.length()){
            ans = center_overlap;
        }
    }

    if (ans.length() <= find_left.first.length()) {
        ans = find_left.first;
    }
    if (ans.length() < find_right.first.length()) {
        ans = find_right.first;
    }

    int x = INT_MIN;
    for (int i = left; i < right; ++i) {
        x = std::max(x, intervals[i].right);
    }

    return std::make_pair(ans, x);
}
