#include <iostream>
#include <vector>

int main() {
        int n;
        std::cin >> n;
        
        std::vector<double> har(n);
        for (int i = 1; i <= n; i++) {
                har[i-1] = 1.0/i;
                std::cout << i << " "
                        << std::scientific << har[i-1] << std::endl;
        }
        return 0;
}
