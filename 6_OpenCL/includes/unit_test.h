#pragma once

#include "bsort.h"

namespace UnitTest {

class bitonic_test final {
    unsigned num;
    unsigned size;
    OpenCL::OpenCL_app app;

    std::vector<float> generate_sequence() const;

    bool check_answer(const std::vector<float> &sequence) const;

    public:
    bitonic_test(unsigned num_ = 10, unsigned size_ = 1048576) : num{num_}, size{size_} {}
    bitonic_test(const bitonic_test&) = delete;
    bitonic_test &operator=(const bitonic_test&) = delete;
    bitonic_test(bitonic_test&&) = delete;
    bitonic_test &operator=(bitonic_test&&) = delete;

    void run();
};

} // UnitTest