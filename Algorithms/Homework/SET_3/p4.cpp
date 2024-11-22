#include <iostream>
#include <vector>
using std::cin;
using std::cout;
using std::vector;
using std::pair;

void stable(vector<pair<int, int> >& A, int left, int mid, int right);
void stableSort(vector<pair<int, int> >& A, int left, int right);

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int> > A;
    for (int i = 0; i < n; ++i) {
        std::string fraction;
        cin >> fraction;
        size_t pos = fraction.find('/');
        int numenator = std::stoi(fraction.substr(0, pos));
        int denominator = std::stoi(fraction.substr(pos + 1));
        A.push_back(std::make_pair(numenator, denominator));
    }

    stableSort(A, 0, n - 1);

    for (int i = 0; i < n; ++i) {
        cout << A[i].first << '/' << A[i].second << ' ';
    }

    return 0;
}

void stable(vector<pair<int, int> >& A, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<pair<int, int> > B(n1);
    vector<pair<int, int> > C(n2);

    for (int i = 0; i < n1; ++i) {
        B[i] = A[left + i];
    }
    for (int j = 0; j < n2; ++j) {
        C[j] = A[mid + j + 1];
    }

    int i = 0;
    int j = 0;
    int k = left;
    while (i < n1 && j < n2) {
        if (B[i].first * C[j].second <= C[j].first * B[i].second) {
            A[k++] = B[i++];
        } else {
            A[k++] = C[j++];
        }
    }
    while (i < n1) {
        A[k++] = B[i++];
    }
    while (j < n2) {
        A[k++] = C[j++];
    }
}

void stableSort(vector<pair<int, int> >& A, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        stableSort(A, left, mid);
        stableSort(A, mid + 1, right);
        stable(A, left, mid, right);
    }
}