#include "input.h"
#include <limits>
#include <iostream>
#include <exception>

namespace {

void cls() {
    std::cout << "Incorrect input" << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

template <typename T>
T get() {
    T val;
    std::cin >> val;

    while (!(std::cin && (std::isspace(std::cin.peek()) || std::cin.eof()))) {
        cls();
        std::cin >> val;
    }
    return val;
}

}

namespace get_data {

std::pair<unsigned, std::vector<double>> get_data() {
    if ((std::cin >> std::ws).eof()) {
        throw std::runtime_error("empty file");
    }

    int rank = get<int>();
    while (rank < 1) {
        std::cout << "Matrix order should be > 0" << std::endl;
        cls();
        rank = get<int>();
    }

    std::vector<double> input(rank * rank);

    for (auto &elem : input) {
        if ((std::cin >> std::ws).eof()) 
            throw std::runtime_error("not enough data to create a matrix");
        elem = get<double>();
    }

    return {rank, input};
}

}