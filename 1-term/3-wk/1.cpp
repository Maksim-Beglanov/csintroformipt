#include <iostream>
#include <iomanip>

int main() {
        const long long N = 10000;
        double res = 1.0;
        double term = 0.5;
        for (int i = 1; i < N; i++) {
                res -= term;
                term /= 2;
        }
        std::cout << res << std::endl;
        return 0;
}
