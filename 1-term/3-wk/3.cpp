#include <iostream>

const int N = 10000000;

double sin(double x) {
        double term = x;
        double res = 0.0;
        for (int i = 1; i < N; i++) {
                if (i % 2 == 1)
                        res += term;
                else
                        res -= term;
                term /= 2*i;
                term *= x;
                term /= 2*i + 1;
                term *= x;
        }
        return res;
}

int main() {
        std::cout << sin(3.14159265358979323846) << std::endl;
        for (int i = 0; i < N; i++) {
                double x = static_cast<double>(i);
                std::cout << sin(x) << std::endl;
        }
        return 0;
}
