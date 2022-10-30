#include "matrix.h"
#include <gtest/gtest.h>

namespace matrix_unit_tests {

const double accurasy = 0.0001;

bool isEqual(const double lhs, const double rhs) {
    if (std::abs(lhs - rhs) < accurasy)
        return 1;
    return 0;
}

class matrix_test : public ::testing::Test {
    protected:
    void SetUp() override {
        std::vector<double> elems{4, 6, -2, 4, 1, 2, -3, 1, 4, -2, 1, 0, 6, 4, 4, 6};
        matrix = new matrix::matrix_t(elems, 4);
    }

    void TearDown() override {
        delete matrix;
    }

    matrix::matrix_t *matrix;
};

TEST_F(matrix_test, get_element) {
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 4));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], 6));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], -2));
    EXPECT_EQ(1, isEqual((*matrix)[1][4], 4));
    EXPECT_EQ(1, isEqual((*matrix)[2][1], 1));
    EXPECT_EQ(1, isEqual((*matrix)[2][2], 2));
    EXPECT_EQ(1, isEqual((*matrix)[2][3], -3));
    EXPECT_EQ(1, isEqual((*matrix)[2][4], 1));
    EXPECT_EQ(1, isEqual((*matrix)[3][1], 4));
    EXPECT_EQ(1, isEqual((*matrix)[3][2], -2));
    EXPECT_EQ(1, isEqual((*matrix)[3][3], 1));
    EXPECT_EQ(1, isEqual((*matrix)[3][4], 0));
    EXPECT_EQ(1, isEqual((*matrix)[4][1], 6));
    EXPECT_EQ(1, isEqual((*matrix)[4][2], 4));
    EXPECT_EQ(1, isEqual((*matrix)[4][3], 4));
    EXPECT_EQ(1, isEqual((*matrix)[4][4], 6));
}

TEST_F(matrix_test, row_swap) {
    (*matrix).row_swap(1, 3);
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 4));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], -2));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], 1));
    EXPECT_EQ(1, isEqual((*matrix)[1][4], 0));
    EXPECT_EQ(1, isEqual((*matrix)[3][1], 4));
    EXPECT_EQ(1, isEqual((*matrix)[3][2], 6));
    EXPECT_EQ(1, isEqual((*matrix)[3][3], -2));
    EXPECT_EQ(1, isEqual((*matrix)[3][4], 4));

    (*matrix).row_swap(1, 2);
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 1));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], 2));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], -3));
    EXPECT_EQ(1, isEqual((*matrix)[1][4], 1));
    EXPECT_EQ(1, isEqual((*matrix)[2][1], 4));
    EXPECT_EQ(1, isEqual((*matrix)[2][2], -2));
    EXPECT_EQ(1, isEqual((*matrix)[2][3], 1));
    EXPECT_EQ(1, isEqual((*matrix)[2][4], 0));
}

TEST_F(matrix_test, con_swap) {
    (*matrix).con_swap(1, 3);
    EXPECT_EQ(1, isEqual((*matrix)[1][1], -2));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], 4));
    EXPECT_EQ(1, isEqual((*matrix)[2][1], -3));
    EXPECT_EQ(1, isEqual((*matrix)[2][3], 1));
    EXPECT_EQ(1, isEqual((*matrix)[3][1], 1));
    EXPECT_EQ(1, isEqual((*matrix)[3][3], 4));
    EXPECT_EQ(1, isEqual((*matrix)[4][1], 4));
    EXPECT_EQ(1, isEqual((*matrix)[4][3], 6));

    (*matrix).con_swap(1, 2);
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 6));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], -2));
    EXPECT_EQ(1, isEqual((*matrix)[2][1], 2));
    EXPECT_EQ(1, isEqual((*matrix)[2][2], -3));
    EXPECT_EQ(1, isEqual((*matrix)[3][1], -2));
    EXPECT_EQ(1, isEqual((*matrix)[3][2], 1));
    EXPECT_EQ(1, isEqual((*matrix)[4][1], 4));
    EXPECT_EQ(1, isEqual((*matrix)[4][2], 4));
}

TEST_F(matrix_test, maximum) {
    std::pair<unsigned, unsigned> res = std::make_pair(1, 2);
    EXPECT_EQ(1, (*matrix).maximum(1) == res);

    std::pair<unsigned, unsigned> res1 = std::make_pair(4, 4);
    EXPECT_EQ(1, (*matrix).maximum(2) == res1);
    EXPECT_EQ(1, (*matrix).maximum(3) == res1);
    EXPECT_EQ(1, (*matrix).maximum(4) == res1);
}

TEST_F(matrix_test, row_sub) {
    matrix::row_t lhs{(*matrix)[1]};
    lhs *= 3;
    (*matrix)[3] -= lhs;
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 4));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], 6));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], -2));
    EXPECT_EQ(1, isEqual((*matrix)[1][4], 4));
    EXPECT_EQ(1, isEqual((*matrix)[3][1], -8));
    EXPECT_EQ(1, isEqual((*matrix)[3][2], -20));
    EXPECT_EQ(1, isEqual((*matrix)[3][3], 7));
    EXPECT_EQ(1, isEqual((*matrix)[3][4], -12));

    matrix::row_t lhs1{(*matrix)[2]};
    lhs1 *= 2;
    (*matrix)[1] -= lhs1;
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 2));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], 2));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], 4));
    EXPECT_EQ(1, isEqual((*matrix)[1][4], 2));
    EXPECT_EQ(1, isEqual((*matrix)[2][1], 1));
    EXPECT_EQ(1, isEqual((*matrix)[2][2], 2));
    EXPECT_EQ(1, isEqual((*matrix)[2][3], -3));
    EXPECT_EQ(1, isEqual((*matrix)[2][4], 1));

    matrix::row_t lhs2{(*matrix)[1]};
    lhs2 *= 0;
    (*matrix)[4] -= lhs2;
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 2));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], 2));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], 4));
    EXPECT_EQ(1, isEqual((*matrix)[1][4], 2));
    EXPECT_EQ(1, isEqual((*matrix)[4][1], 6));
    EXPECT_EQ(1, isEqual((*matrix)[4][2], 4));
    EXPECT_EQ(1, isEqual((*matrix)[4][3], 4));
    EXPECT_EQ(1, isEqual((*matrix)[4][4], 6));


    matrix::row_t lhs3{(*matrix)[1]};
    lhs3 *= -1;
    (*matrix)[4] -= lhs3;
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 2));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], 2));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], 4));
    EXPECT_EQ(1, isEqual((*matrix)[1][4], 2));
    EXPECT_EQ(1, isEqual((*matrix)[4][1], 8));
    EXPECT_EQ(1, isEqual((*matrix)[4][2], 6));
    EXPECT_EQ(1, isEqual((*matrix)[4][3], 8));
    EXPECT_EQ(1, isEqual((*matrix)[4][4], 8));
}

#ifdef CHECK_ELIMINATE 
TEST_F(matrix_test, eliminate) {
    (*matrix).eliminate(1);
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 4));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], 6));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], -2));
    EXPECT_EQ(1, isEqual((*matrix)[1][4], 4));
    EXPECT_EQ(1, isEqual((*matrix)[2][1], 0));
    EXPECT_EQ(1, isEqual((*matrix)[2][2], 0.5));
    EXPECT_EQ(1, isEqual((*matrix)[2][3], -2.5));
    EXPECT_EQ(1, isEqual((*matrix)[2][4], 0));
    EXPECT_EQ(1, isEqual((*matrix)[3][1], 0));
    EXPECT_EQ(1, isEqual((*matrix)[3][2], -8));
    EXPECT_EQ(1, isEqual((*matrix)[3][3], 3));
    EXPECT_EQ(1, isEqual((*matrix)[3][4], -4));
    EXPECT_EQ(1, isEqual((*matrix)[4][1], 0));
    EXPECT_EQ(1, isEqual((*matrix)[4][2], -5));
    EXPECT_EQ(1, isEqual((*matrix)[4][3], 7));
    EXPECT_EQ(1, isEqual((*matrix)[4][4], 0));

    (*matrix).eliminate(2);
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 4));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], 6));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], -2));
    EXPECT_EQ(1, isEqual((*matrix)[1][4], 4));
    EXPECT_EQ(1, isEqual((*matrix)[2][1], 0));
    EXPECT_EQ(1, isEqual((*matrix)[2][2], 0.5));
    EXPECT_EQ(1, isEqual((*matrix)[2][3], -2.5));
    EXPECT_EQ(1, isEqual((*matrix)[2][4], 0));
    EXPECT_EQ(1, isEqual((*matrix)[3][1], 0));
    EXPECT_EQ(1, isEqual((*matrix)[3][2], 0));
    EXPECT_EQ(1, isEqual((*matrix)[3][3], -37));
    EXPECT_EQ(1, isEqual((*matrix)[3][4], -4));
    EXPECT_EQ(1, isEqual((*matrix)[4][1], 0));
    EXPECT_EQ(1, isEqual((*matrix)[4][2], 0));
    EXPECT_EQ(1, isEqual((*matrix)[4][3], -18));
    EXPECT_EQ(1, isEqual((*matrix)[4][4], 0));

    (*matrix).eliminate(3);
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 4));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], 6));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], -2));
    EXPECT_EQ(1, isEqual((*matrix)[1][4], 4));
    EXPECT_EQ(1, isEqual((*matrix)[2][1], 0));
    EXPECT_EQ(1, isEqual((*matrix)[2][2], 0.5));
    EXPECT_EQ(1, isEqual((*matrix)[2][3], -2.5));
    EXPECT_EQ(1, isEqual((*matrix)[2][4], 0));
    EXPECT_EQ(1, isEqual((*matrix)[3][1], 0));
    EXPECT_EQ(1, isEqual((*matrix)[3][2], 0));
    EXPECT_EQ(1, isEqual((*matrix)[3][3], -37));
    EXPECT_EQ(1, isEqual((*matrix)[3][4], -4));
    EXPECT_EQ(1, isEqual((*matrix)[4][1], 0));
    EXPECT_EQ(1, isEqual((*matrix)[4][2], 0));
    EXPECT_EQ(1, isEqual((*matrix)[4][3], 0));
    EXPECT_EQ(1, isEqual((*matrix)[4][4], 1.94594595));
}
#endif

TEST_F(matrix_test, determinant) {
    EXPECT_EQ(1, isEqual((*matrix).determinant(), -144));
}

} // matrix_unit_tests