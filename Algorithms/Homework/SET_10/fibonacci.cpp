#include <iostream>
#include <cmath>
using std::cin;
using std::cout;
using std::vector;

const long long MOD = static_cast<long long>(std::pow(10, 9) + 7);

int main() {
    int n;
    cin >> n;

    long long p1 = 1;
    long long p2 = 2;
    long long cur;
    for (int i = 0; i < n; ++i) {
        cur = (p1 + p2) % MOD;
        p1 = p2;
        p2 = cur;
    }

    cout << p1 << '\n';
    return 0;
}
