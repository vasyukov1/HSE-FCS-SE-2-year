#include <iostream>
#include <vector>
using std::cin;
using std::cout;
using std::vector;
using std::string;

string hexCharToBin(char c);
string hexToBin(string hex);
void makeMatrix(vector<vector<int> >& matrix, int n);
vector<vector<int> > multiply(const vector<vector<int> >& A, const vector<vector<int> >& B, int n);
bool areMatricesEqual(const vector<vector<int> >& A, const vector<vector<int> >& B, int n);

int main() { 
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int> > A(n, vector<int>(n, 0));
    vector<vector<int> > B(n, vector<int>(n, 0));
    vector<vector<int> > C(n, vector<int>(n, 0));
    makeMatrix(A, n);
    makeMatrix(B, n);
    makeMatrix(C, n);

    if (areMatricesEqual(C, multiply(A, B, n), n)) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }

    return 0;
}

string hexCharToBin(char c) {
    switch (c) {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
        default: return "";
    }
}

string hexToBin(string hex) {
    string bin = "";
    for (int i = 0; i < hex.length(); ++i) {
        bin += hexCharToBin(hex[i]);
    }
    return bin;
}

void makeMatrix(vector<vector<int> >& matrix, int n) {
    for (int i = 0; i < n; ++i) {
        string num16;
        cin >> num16;
        string num2 = hexToBin(num16);
        for (int j = 0; j < n; ++j) {
            int el = num2[j] - '0';
            matrix[i][j] = el;
        }
    }
}

vector<vector<int> > multiply(const vector<vector<int> >& A, const vector<vector<int> >& B, int n) {
    vector<vector<int> > C(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int sum = 0;
            for (int k = 0; k < n; ++k) {
                sum ^= A[i][k] & B[k][j];
            }
            C[i][j] = sum;
        }
    }
    return C;
}

bool areMatricesEqual(const vector<vector<int> >& A, const vector<vector<int> >& B, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (A[i][j] != B[i][j]) {
                return false;
            }
        }
    }
    return true;
}
