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
        std::vector<double> elems{1, 2, 3, 4, 5, 6, 7, 8, 9};
        matrix = new matrix::matrix_t(elems, 3);
    }

    void TearDown() override {
        delete matrix;
    }

    matrix::matrix_t *matrix;
};

TEST_F(matrix_test, get_element) {
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 1));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], 2));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], 3));
    EXPECT_EQ(1, isEqual((*matrix)[2][1], 4));
    EXPECT_EQ(1, isEqual((*matrix)[2][2], 5));
    EXPECT_EQ(1, isEqual((*matrix)[2][3], 6));
    EXPECT_EQ(1, isEqual((*matrix)[3][1], 7));
    EXPECT_EQ(1, isEqual((*matrix)[3][2], 8));
    EXPECT_EQ(1, isEqual((*matrix)[3][3], 9));
}

TEST_F(matrix_test, row_swap) {
    (*matrix).row_swap(1, 3);
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 7));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], 8));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], 9));
    EXPECT_EQ(1, isEqual((*matrix)[3][1], 1));
    EXPECT_EQ(1, isEqual((*matrix)[3][2], 2));
    EXPECT_EQ(1, isEqual((*matrix)[3][3], 3));

    (*matrix).row_swap(1, 2);
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 4));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], 5));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], 6));
    EXPECT_EQ(1, isEqual((*matrix)[2][1], 7));
    EXPECT_EQ(1, isEqual((*matrix)[2][2], 8));
    EXPECT_EQ(1, isEqual((*matrix)[2][3], 9));
}

TEST_F(matrix_test, con_swap) {
    (*matrix).con_swap(1, 3);
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 3));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], 1));
    EXPECT_EQ(1, isEqual((*matrix)[2][1], 6));
    EXPECT_EQ(1, isEqual((*matrix)[2][3], 4));
    EXPECT_EQ(1, isEqual((*matrix)[3][1], 9));
    EXPECT_EQ(1, isEqual((*matrix)[3][3], 7));

    (*matrix).con_swap(1, 2);
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 2));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], 3));
    EXPECT_EQ(1, isEqual((*matrix)[2][1], 5));
    EXPECT_EQ(1, isEqual((*matrix)[2][2], 6));
    EXPECT_EQ(1, isEqual((*matrix)[3][1], 8));
    EXPECT_EQ(1, isEqual((*matrix)[3][2], 9));
}

TEST_F(matrix_test, maximum) {
    std::pair<unsigned, unsigned> res = std::make_pair(3, 3);
    EXPECT_EQ(1, (*matrix).maximum(1) == res);
    EXPECT_EQ(1, (*matrix).maximum(2) == res);
    EXPECT_EQ(1, (*matrix).maximum(3) == res);
}

TEST_F(matrix_test, row_sub) {
    (*matrix).row_sub(3, 1, 3);
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 1));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], 2));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], 3));
    EXPECT_EQ(1, isEqual((*matrix)[3][1], 4));
    EXPECT_EQ(1, isEqual((*matrix)[3][2], 2));
    EXPECT_EQ(1, isEqual((*matrix)[3][3], 0));

    (*matrix).row_sub(1, 2, 2);
    EXPECT_EQ(1, isEqual((*matrix)[1][1], -7));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], -8));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], -9));
    EXPECT_EQ(1, isEqual((*matrix)[2][1], 4));
    EXPECT_EQ(1, isEqual((*matrix)[2][2], 5));
    EXPECT_EQ(1, isEqual((*matrix)[2][3], 6));
}

/*TEST_F(matrix_test, eliminate) {
    (*matrix).eliminate(1);
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 1));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], 2));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], 3));
    EXPECT_EQ(1, isEqual((*matrix)[2][1], 0));
    EXPECT_EQ(1, isEqual((*matrix)[2][2], -3));
    EXPECT_EQ(1, isEqual((*matrix)[2][3], -6));
    EXPECT_EQ(1, isEqual((*matrix)[3][1], 0));
    EXPECT_EQ(1, isEqual((*matrix)[3][2], -6));
    EXPECT_EQ(1, isEqual((*matrix)[3][3], -12));

    (*matrix).eliminate(2);
    EXPECT_EQ(1, isEqual((*matrix)[1][1], 1));
    EXPECT_EQ(1, isEqual((*matrix)[1][2], 2));
    EXPECT_EQ(1, isEqual((*matrix)[1][3], 3));
    EXPECT_EQ(1, isEqual((*matrix)[2][1], 0));
    EXPECT_EQ(1, isEqual((*matrix)[2][2], -3));
    EXPECT_EQ(1, isEqual((*matrix)[2][3], -6));
    EXPECT_EQ(1, isEqual((*matrix)[3][1], 0));
    EXPECT_EQ(1, isEqual((*matrix)[3][2], 0));
    EXPECT_EQ(1, isEqual((*matrix)[3][3], 0));
}*/

TEST_F(matrix_test, determinand) {
    EXPECT_EQ(1, isEqual((*matrix).determinand(), 0));
}

} // matrix_unit_tests