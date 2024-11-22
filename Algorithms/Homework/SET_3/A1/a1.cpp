#include <iostream>
#include <vector>
#include <cmath>
#include <random>
using std::cin;
using std::cout;
using std::vector;
using std::pair;
using std::pow;
using std::sqrt;
 
bool f(double cx, double cy, double r, double x, double y);
void areaMonteCarlo(vector<double>& a, vector<double>& b, vector<double>& c, bool method);
 
int main() {
    vector<double> a;
    vector<double> b;
    vector<double> c;
    for (int i = 0; i < 3; ++i) {
        a.push_back(1); 
        a.push_back(1);
        a.push_back(1);    
    }
    for (int i = 0; i < 3; ++i) {
        b.push_back(1.5);
        b.push_back(2);
        b.push_back(sqrt(5) / 2);       
    }
    for (int i = 0; i < 3; ++i) {
        c.push_back(2);
        c.push_back(1.5);
        c.push_back(sqrt(5) / 2);        
    }
    areaMonteCarlo(a, b, c, true);
    return 0;
}
 
bool f(double cx, double cy, double r, double x, double y) {
    return pow((x - cx), 2) + pow((y - cy), 2) <= pow(r, 2);
}
 
void areaMonteCarlo(vector<double>& a, vector<double>& b, vector<double>& c, bool method) {
 
    double x_min, x_max, y_min, y_max;
    if (method) {
        x_min = std::max(std::max(a[0] - a[2], b[0] - b[2]), c[0] - c[2]);
        x_max = std::min(std::min(a[0] + a[2], b[0] + b[2]), c[0] + c[2]);
        y_min = std::max(std::max(a[1] - a[2], b[1] - b[2]), c[1] - c[2]);
        y_max = std::min(std::min(a[1] + a[2], b[1] + b[2]), c[1] + c[2]);
    } else {
        x_min = std::min(std::min(a[0] - a[2], b[0] - b[2]), c[0] - c[2]);
        x_max = std::max(std::max(a[0] + a[2], b[0] + b[2]), c[0] + c[2]);
        y_min = std::min(std::min(a[1] - a[2], b[1] - b[2]), c[1] - c[2]);
        y_max = std::max(std::max(a[1] + a[2], b[1] + b[2]), c[1] + c[2]);
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib_x(x_min, x_max);
    std::uniform_real_distribution<> distrib_y(y_min, y_max);

    for (int i = 100; i <= 100000; i += 500) {
        double M = 0;
 
        for (int j = 0; j < i; ++j) {
            double x = distrib_x(gen);
            double y = distrib_y(gen);
 
            bool circle1 = f(a[0], a[1], a[2], x, y);
            bool circle2 = f(b[0], b[1], b[2], x, y);
            bool circle3 = f(c[0], c[1], c[2], x, y);
 
            if (circle1 && circle2 && circle3) {
                ++M;
            }
        }
        
        double S = (x_max - x_min) * (y_max - y_min);
        double S_rec = (M / i) * S;
        
        cout << i << ' ' << S_rec << '\n';
    }
}