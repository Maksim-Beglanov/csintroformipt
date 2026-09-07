#include <iostream>
#include <array>

int main() {
        int const n = 30;

        std::array<int, n+1> fib = {0, 1};
        int i = 1;
        for (; i < n; i++) {
                std::cout << i << " " << fib[i] << std::endl;
                fib[i+1] = fib[i-1] + fib[i];
        }
        std::cout << i << " " << fib[i] << std::endl;
}
