#include <fstream>
#include <iostream>

int main() {
        int n = 30;
        std::ofstream output("output.tmp", std::ios::binary);

        for (int i = 0; i < n; i++)
                output.write(reinterpret_cast<char*>(&i), sizeof(i));
        output.close();

        std::ifstream input("output.tmp", std::ios::binary);
        int buf;
        for (int i = 0; i < n; i++) {
                input.read(reinterpret_cast<char*>(&buf), sizeof(buf));
                std::cout << buf << std::endl;
        }
        input.close();

        return 0;
}
