#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
        // 5 task
        std::ofstream output("output.tmp");
        int n = std::stoi(argv[1]);
        for (int i = 1; i <= n; i++)
                output << i << " ";

        return 0;
}
