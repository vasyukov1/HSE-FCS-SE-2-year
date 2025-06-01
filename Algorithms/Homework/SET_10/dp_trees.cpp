#include <iostream>
#include <vector>
using std::cin;
using std::cout;
using std::vector;

int main() {
    int n, l, r;
    cin >> n >> l >> r;

    const int len = n;
    vector<int> t(n);

    --l;
    --r;

    int pick = (r + l) / 2;

    t[pick] = n;
    --n;

    int pos = pick;
    int step = 1;
    while (n > 0) {
        pos += step;
        if (pos >= 0 && pos < len) {
            t[pos] = n;
            --n;
        }
        if (step > 0) {
            step = -(step + 1);
        } else {
            step = -(step - 1);
        }
    }

    for (int i = 0; i < len; ++i) {
        cout << t[i] << ' ';
    }

    return 0;
}
