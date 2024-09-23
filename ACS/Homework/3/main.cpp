#include <iostream>
using std::cin;
using std::cout;
using std::string;

void solution(const int&, const int&, const int&);

int main() {

    const int pos_dividend = 15;
    const int neg_dividend = -15;
    const int pos_divisor = 2;
    const int neg_divisor = -2;
    const int zero_divisor = 0;

    solution(pos_dividend, pos_divisor, 1);
    solution(pos_dividend, neg_divisor, 2);
    solution(neg_dividend, pos_divisor, 3);
    solution(neg_dividend, neg_divisor, 4);
    solution(pos_dividend, zero_divisor, 5);
}

void solution(const int& dividend, const int& divisor, const int& counter) {
    cout << "----------\nTEST " << counter << '\n';
    cout << "Dividend: " << dividend << '\n';
    cout << "Divisor: " << divisor << '\n';
    if (divisor == 0) {
        cout << "Division by zero.";
        return;
    }
    const int ceil = dividend / divisor;
    const int remainder = dividend % divisor;
    cout << "Ceil: " << ceil << '\n';
    cout << "Remainder: " << remainder << '\n';
}