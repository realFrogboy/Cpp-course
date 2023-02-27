#include <iostream>
#include <boost/program_options.hpp>

#include "unit_test.h"

namespace {

namespace po = boost::program_options;

bool command_line_arg_handler(const int argc, const char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("test-size", po::value<unsigned>(), "test size")
    ;

    po::positional_options_description p;
    p.add("test-size", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("test-size")) {
        UnitTest::bitonic_test test{1, vm["test-size"].as<unsigned>()};
        test.run();
        return 1;
    }

    return 0;
}

}

int main(const int argc, const char *argv[]) {
    try {
        bool is_tested = command_line_arg_handler(argc, argv);
        if (is_tested) return 0;

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
        test.set_generator_mode<UnitTest::bitonic_test::ascending_generator>();
        test.run();
        test.set_generator_mode<UnitTest::bitonic_test::descending_generator>();
        test.run();
        test.set_generator_mode<UnitTest::bitonic_test::one_number_generator>();
        test.run();
        test.set_n_tests(0);
        test.run();
    } catch (cl::Error &e) {
        std::cout << "Error: " << e.err() << " " << e.what() << std::endl;
    } catch (std::exception & e) {
        std::cout << e.what() << std::endl;
    }
}
