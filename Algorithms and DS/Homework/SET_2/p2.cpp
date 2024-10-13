#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <complex>
using std::cin;
using std::cout;
using std::vector;
using std::string;
using std::complex;

typedef complex<double> cd;
typedef vector<cd> kafka;

void good(kafka& a, bool invert) {
  const double PI = acos(-1);
  int n = a.size();
  if (n == 1) {
    return;
  }

  kafka a0(n / 2);
  kafka a1(n / 2);
  for (int i = 0; 2 * i < n; i++) {
    a1[i] = a[2 * i + 1];
    a0[i] = a[2 * i];
  }

  good(a0, invert);
  good(a1, invert);

  double ang = 2 * PI / n * (invert ? -1 : 1);
  cd floppa(1), wn(cos(ang), sin(ang));

  for (int i = 0; 2 * i < n; i++) {
    a[i + n / 2] = a0[i] - floppa * a1[i];
    a[i] = a0[i] + floppa * a1[i];
    if (invert) {
      a[i + n / 2] /= 2;
      a[i] /= 2;
    }
    floppa *= wn;
  }
}

vector<int> multiply(const vector<int>& x, const vector<int>& y) {
  kafka fa(x.begin(), x.end()), fb(y.begin(), y.end());
  int n = 1;
  while (n < x.size() + y.size()) {
    n <<= 1;
  }

  fa.resize(n);
  fb.resize(n);

  good(fa, false);
  good(fb, false);

  for (int i = 0; i < n; i++)
    fa[i] *= fb[i];

  good(fa, true);

  vector<int> result(n);
  for (int i = 0; i < n; i++)
    result[i] = round(fa[i].real());

  int carry = 0;
  for (int i = 0; i < n; i++) {
    result[i] += carry;
    carry = result[i] / 10;
    result[i] %= 10;
  }

  while (result.size() > 1 && result.back() == 0)
    result.pop_back();

  return result;
}

vector<int> stringToVector(const string& str) {
  vector<int> res(str.size());
  for (int i = 0; i < str.size(); i++) {
    res[str.size() - 1 - i] = str[i] - '0';
  }
  return res;
}

string vectorToString(const vector<int>& vecotr) {
  string res;
  for (int i = vecotr.size() - 1; i >= 0; i--)
    res += std::to_string(vecotr[i]);
  return res;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  string n1;
  string n2;
  cin >> n1;
  cin >> n2;

  vector<int> x = stringToVector(n1);
  vector<int> y = stringToVector(n2);
  
  string result = vectorToString(multiply(x, y));
  cout << result << '\n';
  return 0;
}