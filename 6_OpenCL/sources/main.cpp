#include <chrono>
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

        OpenCL::IOpenCL_app app{};
        auto res = app.bitonic_sort(sequence);
        copy(res.first.begin(), res.first.end(), std::ostream_iterator<float>(std::cout, " "));
        std::cout << "\n\nBitonic sort time: " << res.second << "ns." << std::endl;

        std::chrono::high_resolution_clock::time_point TimeStart, TimeFin;
        TimeStart = std::chrono::high_resolution_clock::now();
        std::sort(sequence.begin(), sequence.end());
        TimeFin = std::chrono::high_resolution_clock::now();
        std::cout << "std::sort time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(TimeFin - TimeStart).count() << "ns." << std::endl;
    } catch (cl::Error &e) {
        std::cout << "Error: " << e.err() << " " << e.what() << std::endl;
    } catch (std::exception & e) {
        std::cout << e.what() << std::endl;
    }
}
