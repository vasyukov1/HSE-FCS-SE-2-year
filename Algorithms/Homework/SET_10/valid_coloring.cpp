#include <iostream>
#include <vector>
using std::cin;
using std::cout;
using std::vector;

bool isValid(const vector<vector<int>>& grid, const int m, const int row) {
    for (int r1 = 0; r1 < row; ++r1) {
        for (int y1 = 0; y1 < m; ++y1) {
            for (int y2 = y1 + 1; y2 < m; ++y2) {
                if (grid[r1][y1] == grid[row][y1] &&
                    grid[r1][y2] == grid[row][y2] &&
                    grid[r1][y1] == grid[r1][y2]) {
                    return false;
                    }
            }
        }
    }
    return true;
}

bool solve(vector<vector<int>>& grid, const int n, const int m, const int c, const int row) {
    if (row == n) {
        return true;
    }

    vector<int> colors(m);
    int total = 1;
    for (int i = 0; i < m; ++i) total *= c;

    for (int mask = 0; mask < total; ++mask) {
        int tmp = mask;
        for (int j = 0; j < m; ++j) {
            colors[j] = tmp % c;
            tmp /= c;
        }

        grid[row] = colors;
        if (isValid(grid, m, row)) {
            if (solve(grid, n, m, c, row + 1)) {
                return true;
            }
        }
    }

    return false;
}

int main() {
    int n, m, c;
    cin >> n >> m >> c;

    vector<vector<int>> grid(n, vector<int>(m, 0));

    if (solve(grid, n, m, c, 0)) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j)
                cout << grid[i][j] + 1 << ' ';
            cout << '\n';
        }
    } else {
        cout << "NO SOLUTION\n";
    }

    return 0;
}
