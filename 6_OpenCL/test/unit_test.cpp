#include <random>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "unit_test.h"

namespace UnitTest {

std::vector<float> bitonic_test::generate_sequence() const {
    std::vector<float> sequence(size);
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(-1000.0, 1000.0);
    std::generate(sequence.begin(), sequence.end(), [&dist, &mt]{return dist(mt);});
    return sequence;
}

bool bitonic_test::check_answer(const std::vector<float> &sequence) const {
    int idx = 0;
    if (direction == 0) {
        auto check = std::find_if(std::next(sequence.begin()), sequence.end(), [&idx, &sequence](float curr) {
            return (curr < sequence[idx++]);
        });
        return (check == sequence.end());
    } else {
        auto check = std::find_if(std::next(sequence.begin()), sequence.end(), [&idx, &sequence](float curr) {
            return (curr > sequence[idx++]);
        });
        return (check == sequence.end());
    }
}

void bitonic_test::run() const {
    unsigned correct = 0;
    for (unsigned cnt = 1; cnt <= num; ++cnt) {
        std::vector<float> sequence = generate_sequence();
        std::vector<float> res = app.bitonic_sort(sequence);
        bool check = check_answer(res);
        if (!check)
            std::cout << "Incorrect answer in the test #" << num << std::endl;
        correct += check;
    }
    std::cout << "Number of correct: " << correct << " / " << num << std::endl;
}

} // UnitTest