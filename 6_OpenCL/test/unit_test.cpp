#include <chrono>
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
    (order == OpenCL::Order::ASCENDING) ? r_step = 10.f : l_step = -10.f;
    float step = get_random_number(l_step, r_step);

    std::generate(sequence.begin(), sequence.end(), [&start, step]{ return start += step; });
    return sequence;
}

std::vector<float> bitonic_test::one_number_generator::generate_sequence() {
    float number = get_random_number(l_board, r_board);
    std::vector<float> sequence(size, number);
    return sequence;
}

bool bitonic_test::check_answer(const std::vector<float> &sequence) const {
    if (app.direction() == 0) 
        return std::is_sorted(sequence.begin(), sequence.end());
    return std::is_sorted(sequence.rbegin(), sequence.rend());
}

void bitonic_test::run() {
    unsigned correct = 0;
    std::cout << "Mode: " << generator_ptr->get_mode() << std::endl;
    std::cout << "Sequence size: " << size << std::endl;
    for (unsigned cnt = 1; cnt <= num; ++cnt) {
        std::vector<float> sequence = generator_ptr->generate_sequence();
        auto [res, total_time, gpu_time] = app.bitonic_sort(sequence);

        bool check = check_answer(res);
        if (!check)
            std::cout << "Incorrect answer in the test #" << num << std::endl;
        else {
            std::chrono::high_resolution_clock::time_point TimeStart, TimeFin;
            TimeStart = std::chrono::high_resolution_clock::now();
            std::sort(sequence.begin(), sequence.end());
            TimeFin = std::chrono::high_resolution_clock::now();
            std::cout << "Test #" << cnt << ": Bitonic sort time (total time / host_time / gpu_time):" 
            " " << total_time << "ns. /"
            " " << (total_time - gpu_time) << "ns. /" 
            " " << gpu_time << "ns."
            " VS std::sort time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(TimeFin - TimeStart).count() << "ns." << std::endl;
            ++correct;
        }
    }
    std::cout << "Number of correct: " << correct << " / " << num << "\n" << std::endl;
}

} // UnitTest
