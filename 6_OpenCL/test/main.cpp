#include <iostream>

#include "unit_test.h"

int main() {
    try {
        UnitTest::bitonic_test test{};
        test.run();
    } catch (cl::Error &e) {
        std::cout << "Error: " << e.err() << " " << e.what() << std::endl;
    } catch (std::exception & e) {
        std::cout << e.what() << std::endl;
    }
}