#pragma once

#include <random>

#include "bsort.h"

namespace UnitTest {

class bitonic_test final {

    class generator {
        std::random_device rd;
        std::mt19937 mt;
        std::string s_mode;

        public:
        static constexpr float l_board = -1000.f;
        static constexpr float r_board = 1000.f;
        unsigned size;

        generator(const unsigned size_, const std::string &mode) : mt{rd()}, s_mode{mode}, size{size_} {}

        float get_random_number(const float l_board, const float r_board) { 
            std::uniform_real_distribution<float> dist(l_board, r_board);
            return dist(mt);
        }
        std::string get_mode() { return s_mode; }
        virtual std::vector<float> generate_sequence() = 0;
        virtual ~generator() {}
    };

    class monotonic_generator : public generator {
        OpenCL::Order order;

        public:
        monotonic_generator(const unsigned size_, const std::string &mode, const OpenCL::Order order_) : generator(size_, mode), order{order_} {}
        std::vector<float> generate_sequence() override;
    };

    unsigned num;
    unsigned size;
    OpenCL::IOpenCL_app app;
    std::unique_ptr<generator> generator_ptr;

    bool check_answer(const std::vector<float> &sequence) const;

    public:

    struct random_generator final : generator {
        random_generator(const unsigned size_) : generator(size_, "RANDOM") {}
        std::vector<float> generate_sequence() override;
    };

    struct ascending_generator final : monotonic_generator {
        ascending_generator(const unsigned size_) : monotonic_generator(size_, "ASCENDING", OpenCL::Order::ASCENDING) {}
    };

    struct descending_generator final : monotonic_generator {
        descending_generator(const unsigned size_) : monotonic_generator(size_, "DESCENDING", OpenCL::Order::DESCENDING) {}
    };

    struct one_number_generator final : generator {
        one_number_generator(const unsigned size_) : generator(size_, "ONE_NUMBER") {}
        std::vector<float> generate_sequence() override;
    };

    bitonic_test(const unsigned num_ = 10, const unsigned size_ = 1048576) : 
        num{num_}, size{size_}, generator_ptr{std::make_unique<random_generator>(size_)} {}
    bitonic_test(const bitonic_test&) = delete;
    bitonic_test &operator=(const bitonic_test&) = delete;
    bitonic_test(bitonic_test&&) = delete;
    bitonic_test &operator=(bitonic_test&&) = delete;

    void set_n_tests(const unsigned n) { num = n; }
    void set_test_size(const unsigned sz) { size = sz; }
    
    template <typename T>
    void set_generator_mode() {
        generator_ptr = std::make_unique<T>(size);
    }

    void run();
};

} // UnitTest
