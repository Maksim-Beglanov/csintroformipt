#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
        if (argc != 2) {
                std::cout << "Using: ./prog file_name" << std::endl;
                return 1;
        }
        std::ofstream output("output.tmp");

        int max_n = std::stoi(argv[1]);
        for (int i = 1; i <= n; i++)
                output << i << " ";

        output.close()

        return 0;
}
