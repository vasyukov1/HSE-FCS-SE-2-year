#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <vector>
#include <random>
using std::cin;
using std::cout;
using std::string;
typedef std::vector<unsigned char> vector;
typedef unsigned int uint;

vector hexToBin(string number, uint size, uint digitSize);
std::vector<vector> fillMatrix(uint n, uint m);
vector multiply(std::vector<vector>& matrix1, vector& vec, uint n);
bool verification(std::vector<vector>& A, std::vector<vector>& B, std::vector<vector>& C, uint n);
bool check(std::vector<vector>& A, std::vector<vector>& B, std::vector<vector>& C, uint n);

std::random_device rand_dev;
std::mt19937 generator(rand_dev());
std::uniform_int_distribution<> distr(0, 1);

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    cin >> n;

    int m = n / 4 + (n % 4 != 0 ? 1 : 0);

    std::vector<vector> A = fillMatrix(n, m);
    std::vector<vector> B = fillMatrix(n, m);
    std::vector<vector> C = fillMatrix(n, m);
    
    cout << (check(A, B, C, n) ? "YES" : "NO");
    return 0;
}

vector hexToBin(string number, uint size, uint digitSize) {
    vector binary(size);
    uint count = digitSize << 2;
    uint del = count - size;
    uint index = size - 1;

    for (int i = number.length() - 1; i >= 0 && count > 0; --i) {
        char ch = number[i];
        ch -= (ch <= '9' ? '0' : 'A' - 10);

        for (int j = 0; j < 4 && count > 0; ++j, --count) {
            if (del > 0) {
                --del;
            } else {
                binary[index--] = ch & 1;
            }
            ch >>= 1;
        }
    }
    return binary;
}

std::vector<vector> fillMatrix(uint n, uint m) {
    string number;
    std::vector<vector> matrix(n);
    for (uint i = 0; i < n; ++i) {
        cin >> number;
        matrix[i] = hexToBin(number, n, m);
    }
    return matrix;
}

vector multiply(std::vector<vector>& matrix1, vector& vec, uint n) {
    vector result(n);
    for (uint i = 0; i < n; i++) {
        result[i] = 0;
        for (uint k = 0; k < n; k++) {
            result[i] ^= matrix1[i][k] & vec[k];
        }
    }
    return result;
}

bool verification(std::vector<vector>& A, std::vector<vector>& B, std::vector<vector>& C, uint n) {
    vector random_vector(n);
    for (uint i = 0; i < n; ++i) {
        random_vector[i] = distr(generator);
    }

    vector B_vec = multiply(B, random_vector, n);
    vector X = multiply(A, B_vec, n);
    vector Y = multiply(C, random_vector, n);

    for (uint i = 0; i < n; ++i) {
        if (X[i] != Y[i]) {
            return false;
        }
    }
    return true;
}

bool check(std::vector<vector>& A, std::vector<vector>& B, std::vector<vector>& C, uint n) {
    for (uint i = 0; i < 10; ++i) {
        if (!verification(A, B, C, n)) {
            return false;
        }
    }
    return true;
}