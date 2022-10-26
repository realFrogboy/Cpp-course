#include "matrix.h"
#include "test_generator.h"
#include <gtest/gtest.h>
#include <exception>

namespace end_to_end {

constexpr double accurasy = 0.01;
constexpr unsigned g_rank = 100;

bool isEqual(const double lhs, const double rhs) {
    if (std::abs(lhs - rhs) < accurasy)
        return 1;
    return 0;
}

TEST(end_to_end, determinant) {
    unsigned rank = g_rank;

    test_generator::test_generator(rank);
    std::ifstream fp("Generation.txt");
    if (!fp.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    while (fp >> rank) {
        ASSERT_TRUE(fp);
        std::vector<double> input(rank * rank);
        for (auto& elem : input) {
            fp >> elem;
            ASSERT_TRUE(fp);
        }

        matrix::matrix_t matrix(input, rank);

        double res = matrix.determinant();

        double true_res = 0;
        fp >> true_res;
        ASSERT_TRUE(fp);

        EXPECT_EQ(1, isEqual(res, true_res));
    }
}

} // end_to_end