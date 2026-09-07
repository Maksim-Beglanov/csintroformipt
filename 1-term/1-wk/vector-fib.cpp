#include <iostream>
#include <vector>

int main() {
        int n;
        std::cin >> n;

        std::vector<int> fib(n+1);
        fib[0] = 0;
        fib[1] = 1;

        int i = 1;
        for (; i < n; i++) {
                std::cout << i << " " << fib[i] << std::endl;
                fib[i+1] = fib[i-1] + fib[i];
        }
        std::cout << i << " " << fib[i] << std::endl;
}
