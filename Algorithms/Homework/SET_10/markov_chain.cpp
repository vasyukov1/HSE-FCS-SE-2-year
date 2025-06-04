#include <iostream>
#include <vector>
#define ll long long
using std::cin;
using std::cout;
using std::vector;

using Matrix = vector<vector<ll>>;
const ll MOD = 1e9 + 179;

Matrix matrixMul(const Matrix& a, const Matrix& b) {
    int n = a.size();
    Matrix res(n, vector<ll>(n , 0));
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            for (int j = 0; j < n; ++j) {
                res[i][j] = (res[i][j] + a[i][k] * b[k][j]) % MOD;
            }
        }
    }
    return res;
}

Matrix matrixPow(Matrix base, ll power) {
    int n = base.size();
    Matrix res(n, vector<ll>(n, 0));
    for (int i = 0; i < n; ++i) {
        res[i][i] = 1;
    }

    while (power) {
        if (power & 1) {
            res = matrixMul(res, base);
        }
        base = matrixMul(base, base);
        power >>= 1;
    }
    return res;
}

int idx(int dir, int val, int k) {
    return dir * k + val - 1;
}

int main() {
    ll n;
    int k;
    cin >> n >> k;

    if (n == 1) {
        cout << k << '\n';
        return 0;
    }

    const int size = 2 * k;
    Matrix T(size, vector<ll>(size, 0));
    vector<ll> base(size, 0);

    for (int prev = 1; prev <= k; ++prev) {
        for (int curr = 1; curr <= k; ++curr) {
            if (curr == prev) {
                continue;
            }
            if (curr > prev) {
                base[idx(1, curr, k)]++;
            } else {
                base[idx(0, curr, k)]++;
            }
        }
    }

    for (int prev = 1; prev <= k; ++prev) {
        for (int curr = 1; curr <= k; ++curr) {
            if (curr == prev) {
                continue;
            }
            if (curr > prev) {
                T[idx(1, curr, k)][idx(0, prev, k)] = 1;
            } else {
                T[idx(0, curr, k)][idx(1, prev, k)] = 1;
            }
        }
    }

    Matrix Tpow = matrixPow(T, n - 2);

    ll answer = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            answer = (answer + Tpow[i][j] * base[j]) % MOD;
        }
    }

    cout << answer << '\n';
    return 0;
}
