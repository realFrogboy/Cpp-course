#include "driver.hpp"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "./paracl <<file>>" << std::endl;
        return 0;
    }

    try {
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        file.open(argv[1]);
        yy::driver_t driver(&file);
        driver.parse();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
}