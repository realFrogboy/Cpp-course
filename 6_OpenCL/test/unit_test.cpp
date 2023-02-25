#include <chrono>
#include <memory>
#include <iostream>
#include <algorithm>

#include "unit_test.h"

namespace UnitTest {

std::vector<float> bitonic_test::random_generator::generate_sequence() {
    std::vector<float> sequence(size);
    std::generate(sequence.begin(), sequence.end(), [this]{ return get_random_number(l_board, r_board); });
    return sequence;
}

std::vector<float> bitonic_test::monotonic_generator::generate_sequence() {
    std::vector<float> sequence(size);
    float start = get_random_number(l_board, r_board);

    float l_step = 0.f, r_step = 0.f;
    (order == Order::ASCENDING) ? r_step = 10.f : l_step = -10.f;

    std::generate(sequence.begin(), sequence.end(), [this, &start, l_step, r_step]{ 
        float step = get_random_number(l_step, r_step);
        start += step;
        return start;
    });
    return sequence;
}

std::vector<float> bitonic_test::one_number_generator::generate_sequence() {
    std::vector<float> sequence(size);
    float number = get_random_number(l_board, r_board);
    std::generate(sequence.begin(), sequence.end(), [number]{ return number; });
    return sequence;
}

bool bitonic_test::check_answer(const std::vector<float> &sequence) const {
    int idx = 0;
    if (app.direction() == 0) {
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

void bitonic_test::set_generator_mode(const generator_mode mode) {
    switch (mode) {
        case generator_mode::RANDOM: {
            auto random_gen = std::make_unique<random_generator>(size);
            generator_ptr = std::move(random_gen);
            s_mode = "RANDOM SEQUENCE";
            break;
        }
        case generator_mode::ASCENDING: {
            auto ascending_gen = std::make_unique<monotonic_generator>(size);
            generator_ptr = std::move(ascending_gen);
            s_mode = "ASCENDING SEQUENCE";
            break;
        }
        case generator_mode::DESCENDING: {
            auto descending_gen = std::make_unique<monotonic_generator>(size, Order::DESCENDING);
            generator_ptr = std::move(descending_gen);
            s_mode = "DESCENDING SEQUENCE";
            break;
        }
        case generator_mode::ONE_NUMBER: {
            auto one_number_gen = std::make_unique<one_number_generator>(size);
            generator_ptr = std::move(one_number_gen);
            s_mode = "ONE_NUMBER_SEQUENCE";
        }
    }
}

void bitonic_test::run() {
    unsigned correct = 0;
    std::cout << "Mode: " << s_mode << std::endl;
    std::cout << "Sequence size: " << size << std::endl;
    for (unsigned cnt = 1; cnt <= num; ++cnt) {
        std::vector<float> sequence = generator_ptr->generate_sequence();
        auto res = app.bitonic_sort(sequence);

        bool check = check_answer(res.first);
        if (!check)
            std::cout << "Incorrect answer in the test #" << num << std::endl;
        else {
            std::chrono::high_resolution_clock::time_point TimeStart, TimeFin;
            TimeStart = std::chrono::high_resolution_clock::now();
            std::sort(sequence.begin(), sequence.end());
            TimeFin = std::chrono::high_resolution_clock::now();
            std::cout << "Test #" << cnt << ": Bitonic time:" << res.second << "ns."
                "VS std::sort time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(TimeFin - TimeStart).count() << "ns." << std::endl;
            ++correct;
        }
    }
    std::cout << "Number of correct: " << correct << " / " << num << "\n" << std::endl;
}

} // UnitTest