#include "matrix.h"
#include "test_generator.h"
#include <gtest/gtest.h>

namespace end_to_end {

const double accurasy = 0.01;

bool isEqual(const double lhs, const double rhs) {
    if (std::abs(lhs - rhs) < accurasy)
        return 1;
    return 0;
}

const int g_rang = 100;

TEST(end_to_end, determinand) {
    test_generator::test_generator(g_rang);
    std::ifstream fp("Generation.txt");

    unsigned rang = 0;
    while (fp >> rang) {
        std::vector<double> input(rang * rang);
        for (auto& elem : input)
            assert(fp >> elem);

        matrix::matrix_t matrix(input, rang);

        double res = matrix.determinand();

        double true_res = 0;
        fp >> true_res;

        EXPECT_EQ(1, isEqual(res, true_res));
    }
}

} // end_to_end