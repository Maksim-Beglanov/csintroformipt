#include <iostream>

int main() {
        const long long N = 1e10;
        float res = 0.0;
        float past_res = res;
        for (long long i = 1; i < N; i++) {
                float term = 1.0/i;
                past_res = res;
                res += term;
                if ((res - past_res) < 1e-12)
                        std::cout << "Check" << std::endl;
        }
        return 0;
}
