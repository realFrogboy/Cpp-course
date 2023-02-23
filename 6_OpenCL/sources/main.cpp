#include <iostream>
#include <iterator>
#include <algorithm>

#include "bsort.h"

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
std::vector<float> get_sequence(const unsigned &size) {
    std::vector<float> sequence(size);
    std::for_each(sequence.begin(), sequence.end(), [](float &curr) {
        curr = get<float>();
    });
    return sequence;
}

} // namespace

int main() {
    try {
        unsigned size = get<unsigned>();
        std::vector<float> sequence = get_sequence(size);
        OpenCL::OpenCL_app app{};
        std::vector<float> res = app.bitonic_sort(sequence);
        copy(res.begin(), res.end(), std::ostream_iterator<float>(std::cout, " "));
    } catch (cl::Error &e) {
        std::cout << "Error: " << e.err() << " " << e.what() << std::endl;
    } catch (std::exception & e) {
        std::cout << e.what() << std::endl;
    }
}
