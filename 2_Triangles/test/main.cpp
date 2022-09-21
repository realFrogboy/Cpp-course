#include <cstdlib>

int main() {
    std::system("./generation 10000");
    std::system("./triag < Generation.txt");

    return 0;
}
