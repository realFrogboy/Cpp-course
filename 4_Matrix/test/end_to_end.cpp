#include "matrix.h"
#include "test_generator.h"
#include <gtest/gtest.h>

namespace end_to_end {

const double accurasy = 0.01;
const unsigned g_rank = 100;

bool isEqual(const double lhs, const double rhs) {
    if (std::abs(lhs - rhs) < accurasy)
        return 1;
    return 0;
}

TEST(end_to_end, determinant) {
    unsigned rank = g_rank;

    test_generator::test_generator(rank);
    std::ifstream fp("Generation.txt");

    while (fp >> rank) {
        assert(fp);
        std::vector<double> input(rank * rank);
        for (auto& elem : input) {
            fp >> elem;
            assert(fp);
        }

        matrix::matrix_t matrix(input, rank);

        double res = matrix.determinant();

        double true_res = 0;
        fp >> true_res;
        assert(fp);

        EXPECT_EQ(1, isEqual(res, true_res));
    }
}

} // end_to_end