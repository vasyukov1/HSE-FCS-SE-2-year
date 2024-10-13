#include <iostream>
#include <vector>
using std::cin;
using std::cout;
using std::vector;

vector<vector<int64_t>> plus(const vector<vector<int64_t>>& A, const vector<vector<int64_t>>& B);
vector<vector<int64_t>> multiply(const vector<vector<int64_t>>& A, const vector<vector<int64_t>>& B);

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int64_t>> A(n, vector<int64_t>(n));
    vector<vector<int64_t>> B(n, vector<int64_t>(n));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            cin >> A[i][j];
        }
    }

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            cin >> B[i][j];
        }
    }

    vector<vector<int64_t>> C = multiply(A, B);
    for (size_t i = 0; i < n; ++i) {
        for(size_t j = 0; j < n; ++j) {
            cout << C[i][j] << ' ';
        }
        cout << '\n';
    }
}

vector<vector<int64_t>> plus(const vector<vector<int64_t>>& A, const vector<vector<int64_t>>& B) {
    int n = A.size();
    vector<vector<int64_t>> C(n, vector<int64_t>(n));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

vector<vector<int64_t>> multiply(const vector<vector<int64_t>>& A, const vector<vector<int64_t>>& B) {
    int n = A.size();
    vector<vector<int64_t>> C(n, vector<int64_t>(n));
    if (n <= 64) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                int64_t sum = 0;
                for (size_t k = 0; k < n; ++k) {
                    sum += A[i][k] * B[k][j];
                }
                C[i][j] = sum;
            }
        }
    } else {
        vector<vector<int64_t>> A11(n / 2, vector<int64_t>(n / 2));
        for (size_t i = 0; i < n / 2; ++i) {
            for (size_t j = 0; j < n / 2; ++j) {
                A11[i][j] = A[i][j];
            }
        }

        vector<vector<int64_t>> A12(n / 2, vector<int64_t>(n / 2));
        for (size_t i = 0; i < n / 2; ++i) {
            for (size_t j = n / 2; j < n; ++j) {
                A12[i][j - n / 2] = A[i][j];
            }
        }

        vector<vector<int64_t>> A21(n / 2, vector<int64_t>(n / 2));
        for (size_t i = n / 2; i < n; ++i) {
            for (size_t j = 0; j < n / 2; ++j) {
                A21[i - n / 2][j] = A[i][j];
            }
        }

        vector<vector<int64_t>> A22(n / 2, vector<int64_t>(n / 2));
        for (size_t i = n / 2; i < n; ++i) {
            for (size_t j = n / 2; j < n; ++j) {
                A22[i - n / 2][j - n / 2] = A[i][j];
            }
        }

        vector<vector<int64_t>> B11(n / 2, vector<int64_t>(n / 2));
        for (size_t i = 0; i < n / 2; ++i) {
            for (size_t j = 0; j < n / 2; ++j) {
                B11[i][j] = B[i][j];
            }
        }

        vector<vector<int64_t>> B12(n / 2, vector<int64_t>(n / 2));
        for (size_t i = 0; i < n / 2; ++i) {
            for (size_t j = n / 2; j < n; ++j) {
                B12[i][j - n / 2] = B[i][j];
            }
        }

        vector<vector<int64_t>> B21(n / 2, vector<int64_t>(n / 2));
        for (size_t i = n / 2; i < n; ++i) {
            for (size_t j = 0; j < n / 2; ++j) {
                B21[i - n / 2][j] = B[i][j];
            }
        }

        vector<vector<int64_t>> B22(n / 2, vector<int64_t>(n / 2));
        for (size_t i = n / 2; i < n; ++i) {
            for (size_t j = n / 2; j < n; ++j) {
                B22[i - n / 2][j - n / 2] = B[i][j];
            }
        }

        vector<vector<int64_t>> tmp;

        vector<vector<int64_t>> C11 = multiply(A11, B11);
        tmp = multiply(A12, B21);
        C11 = plus(C11, tmp);

        vector<vector<int64_t>> C12 = multiply(A11, B12);
        tmp = multiply(A12, B22);
        C12 = plus(C12, tmp);

        vector<vector<int64_t>> C21 = multiply(A21, B11);
        tmp = multiply(A22, B21);
        C21 = plus(C21, tmp);

        vector<vector<int64_t>> C22 = multiply(A21, B12);
        tmp = multiply(A22, B22);
        C22 = plus(C22, tmp);

        for (size_t i = 0 ; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (i < n / 2) {
                    if (j < n / 2) {
                        C[i][j] = C11[i][j];
                    } else {
                        C[i][j] = C12[i][j - n / 2];
                    }
                } else {
                    if (j < n / 2) {
                        C[i][j] = C21[i - n / 2][j];
                    } else {
                        C[i][j] = C22[i - n / 2][j - n / 2];
                    }
                }
            }
        }
    }
    return C;
}