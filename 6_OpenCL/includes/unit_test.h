#pragma once

#include <vector>

#include "bsort.h"

namespace UnitTest {

class bitonic_test final {
    unsigned num;
    unsigned size;
    unsigned direction; /* Ascending: 0, Descending: -1 */
    OpenCL::OpenCL_app app;

    std::vector<float> generate_sequence() const;

    bool check_answer(const std::vector<float> &sequence) const;

    public:
    bitonic_test(unsigned num_ = 10, unsigned size_ = 1048576, unsigned dir = 0) : num{num_}, size{size_}, direction{dir} {}
    bitonic_test(const bitonic_test&) = delete;
    bitonic_test &operator=(const bitonic_test&) = delete;
    bitonic_test(bitonic_test&&) = delete;
    bitonic_test &operator=(bitonic_test&&) = delete;

    void run() const;
};

} // UnitTest