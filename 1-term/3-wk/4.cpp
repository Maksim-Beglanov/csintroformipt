#include <iostream>
#include <cmath>

double f(double x) {
        return std::exp(-x*x);
}

double integr(double d) {
        double res = 0.0;
        double prev_res = res;
        double x = 0.0;
        while (true) {
                prev_res = res;
                res += d * (f(x) + f(x + d))/2;
                if ((res - prev_res) < 1e-12)
                        break;
                x += d;
        }
        return res;
}

int main() {
        for (int i = 5; i < 10000; i += 5) {
                double d = std::sqrt(std::acos(-1.0))/2 -
                        integr(1.0/i);
                std::cout << i << " " << d << std::endl;
        }
}
