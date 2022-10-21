#include "input.h"
#include <iostream>

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
    while(!std::cin) {
        cls();
        std::cin >> val;
    }
    return val;
}

}

namespace get_data{

std::pair<unsigned, std::vector<double>> get_data() {
    unsigned rank = get<unsigned>();

    std::vector<double> input(rank * rank);

    for (auto& elem : input)
        elem = get<double>();

    return {rank, input};
}

}