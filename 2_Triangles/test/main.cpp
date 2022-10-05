#include <cstdlib>

int main() {
    std::system("./generation 1000");
    std::system("./triag < Generation.txt");

    return 0;
}
