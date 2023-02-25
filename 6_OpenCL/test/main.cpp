#include <iostream>

#include "unit_test.h"

int main() {
    try {
        UnitTest::bitonic_test test{5, 3000};
        test.run();
        test.set_test_size(4000);
        test.run();
        test.set_test_size(5000);
        test.run();
        test.set_test_size(6000);
        test.run();
        test.set_test_size(0);
        test.run();
        test.set_n_tests(10);
        test.set_test_size(100000);
        test.set_generator_mode(UnitTest::bitonic_test::generator_mode::ASCENDING);
        test.run();
        test.set_generator_mode(UnitTest::bitonic_test::generator_mode::DESCENDING);
        test.run();
        test.set_generator_mode(UnitTest::bitonic_test::generator_mode::ONE_NUMBER);
        test.run();
        test.set_n_tests(0);
        test.run();
    } catch (cl::Error &e) {
        std::cout << "Error: " << e.err() << " " << e.what() << std::endl;
    } catch (std::exception & e) {
        std::cout << e.what() << std::endl;
    }
}

/*UnitTest::bitonic_test test{5, 3000};
        test.run();
        test.set_test_size(4000);
        test.run();
        test.set_test_size(5000);
        test.run();
        test.set_test_size(6000);
        test.run();
        test.set_test_size(0);
        test.run();
        test.set_n_tests(10);
        test.set_test_size(100000);
        test.set_generator_mode(UnitTest::bitonic_test::generator_mode::ASCENDING);
        test.run();
        test.set_generator_mode(UnitTest::bitonic_test::generator_mode::DESCENDING);
        test.run();
        test.set_generator_mode(UnitTest::bitonic_test::generator_mode::ONE_NUMBER);
        test.run();*/