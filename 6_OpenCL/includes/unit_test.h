#pragma once

#include <random>

#include "bsort.h"

namespace UnitTest {

class bitonic_test final {

    class generator {
        std::random_device rd;
        std::mt19937 mt;

        public:
        static constexpr float l_board = -1000.f;
        static constexpr float r_board = 1000.f;
        unsigned size;

        generator(const unsigned size_) : mt{rd()}, size{size_} {}

        float get_random_number(const float l_board, const float r_board) { 
            std::uniform_real_distribution<float> dist(l_board, r_board);
            return dist(mt);
        }
        virtual std::vector<float> generate_sequence() = 0;
        virtual ~generator() {}
    };

    struct random_generator final : generator {
        random_generator(const unsigned size_) : generator(size_) {}
        std::vector<float> generate_sequence() override;
    };

    enum class Order {
        ASCENDING,
        DESCENDING
    };
    class monotonic_generator final : public generator {
        Order order;

        public:
        monotonic_generator(const unsigned size_, const Order order_ = Order::ASCENDING) : 
            generator(size_), order{order_} {}
        std::vector<float> generate_sequence() override;
    };

    struct one_number_generator final : generator {
        one_number_generator(const unsigned size_) : generator(size_) {}
        std::vector<float> generate_sequence() override;
    };

    unsigned num;
    unsigned size;
    OpenCL::IOpenCL_app app;
    std::unique_ptr<generator> generator_ptr;
    std::string s_mode;

    bool check_answer(const std::vector<float> &sequence) const;

    public:
    enum class generator_mode {
        RANDOM,
        ASCENDING,
        DESCENDING,
        ONE_NUMBER
    };

    bitonic_test(const unsigned num_ = 10, const unsigned size_ = 1048576) : 
        num{num_}, size{size_}, generator_ptr{std::make_unique<random_generator>(size_)}, s_mode{"RANDOM"} {}
    bitonic_test(const bitonic_test&) = delete;
    bitonic_test &operator=(const bitonic_test&) = delete;
    bitonic_test(bitonic_test&&) = delete;
    bitonic_test &operator=(bitonic_test&&) = delete;

    void set_n_tests(const unsigned n) { num = n; }
    void set_test_size(const unsigned sz) { size = sz; }
    void set_generator_mode(const generator_mode mode);

    void run();
};

} // UnitTest