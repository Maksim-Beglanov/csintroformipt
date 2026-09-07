#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
        std::ofstream output("output.tmp");

        int n = std::stoi(argv[1]);
        for (int i = 1; i <= n; i++)
                output << i << " ";

        output.close()

        return 0;
}
