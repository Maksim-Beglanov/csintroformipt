#include <iostream>
#include <array>

int main() {
        const int n = 30;
        
        std::array<double, n> har;
        for (int i = 1; i <= n; i++) {
                har[i-1] = 1.0/i;
                std::cout << i << " "
                        << std::scientific << har[i-1] << std::endl;
        }
        return 0;
}
