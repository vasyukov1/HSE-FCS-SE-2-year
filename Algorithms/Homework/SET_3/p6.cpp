#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <bitset>

using std::cin;
using std::cout;
using std::vector;
using std::bitset;

bool isPossible(int r, int c, bitset<200>& col, bitset<400>& diag1, bitset<400>& diag2) {
    int index1 = 200 + r - c;
    int index2 = r + c;

    return !col[r] && !diag1[index1] && !diag2[index2];
}

bool place(int current, int n, vector<int>& queens,
            bitset<200>& col, std::mt19937& gen,
            bitset<400>& diag1, bitset<400>& diag2) {
    if (current == n) {
        return true;
    }

    vector<int> row(n);
    for (int i = 0; i < n; ++i) {
        row[i] = i;
    }
    shuffle(row.begin(), row.end(), gen);

    for (int i = 0; i < n; ++i) {
        if (isPossible(row[i], current, col, diag1, diag2)) {
            queens[current] = row[i];
            col[row[i]] = true;
            diag1[row[i] - current + 200] = true;
            diag2[row[i] + current] = true;

            if (place(current + 1, n, queens, col, gen, diag1, diag2)) {
                return true;
            }

            col[row[i]] = false;
            diag1[row[i] - current + 200] = false;
            diag2[row[i] + current] = false;
        }
    }

    return false;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> queens(n);
    std::mt19937 gen(42);

    bitset<200> col;
    bitset<400> diag1;
    bitset<400> diag2;

    if (place(0, n, queens, col, gen, diag1, diag2)) {
        for (int i = 0; i < n; ++i) {
            cout << 1 + queens[i] << ' ';
        }
        cout << '\n';
    }

    return 0;
}