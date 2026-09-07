#include <iostream>
#include <fstream>

int main() {
        std::ofstream output("output.tmp");
        int n;
        std::cin >> n;

        int prev, cur;
        prev = 0;
        cur = 1;
        for (int i = 1; i <= n; i++) {
                output << i << " " << cur << std::endl;
                cur = cur + prev;
                prev = cur - prev;
        }
        return 0;
}
