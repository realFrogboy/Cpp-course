#include <vector>
#include <gtest/gtest.h>

#include "bsort.h"

namespace GTests {

struct bitonic_sort : ::testing::Test {
    OpenCL::IOpenCL_app app{};
};

TEST_F(bitonic_sort, test3) {
    std::vector<float> sequence {5.f, 3.f, 1.f};
    std::vector<float> correct  {1.f, 3.f, 5.f};

    auto res = app.bitonic_sort(sequence);
    EXPECT_EQ(1, std::get<0>(res) == correct);
}

TEST_F(bitonic_sort, test6) {
    std::vector<float> sequence {3.f, 5.f, 4.f, 1.f, 2.f, 6.f};
    std::vector<float> correct  {1.f, 2.f, 3.f, 4.f, 5.f, 6.f};

    auto res = app.bitonic_sort(sequence);
    EXPECT_EQ(1, std::get<0>(res) == correct);
}

TEST_F(bitonic_sort, test32) {
    std::vector<float> sequence {664.311, 618.196, -833.805, 522.744, 673.402, 998.018, -380.45, 675.781, 469.587, 261.689, -195.714, -987.415, 346.982, 390.831, 565.423, 557.58, -909.149, -869.018, 311.662, 341.043, -913.056, -313.805, -634.683, 874.578, -387.027, -152.506, -802.989, -788.433, 829.813, -823.499, -542.377, 749.585};
    std::vector<float> correct  {-987.415, -913.056, -909.149, -869.018, -833.805, -823.499, -802.989, -788.433, -634.683, -542.377, -387.027, -380.45, -313.805, -195.714, -152.506, 261.689, 311.662, 341.043, 346.982, 390.831, 469.587, 522.744, 557.58, 565.423, 618.196, 664.311, 673.402, 675.781, 749.585, 829.813, 874.578, 998.018};

    auto res = app.bitonic_sort(sequence);
    EXPECT_EQ(1, std::get<0>(res) == correct);
}

TEST_F(bitonic_sort, test64) {
    std::vector<float> sequence {-205.312, -446.874, 668.151, 224.369, 335.916, 732.115, 993.383, -103.326, -410.843, -60.4781, -174.688, 162.026, 362.071, -682.397, 944.08, 286.279, 711.33, -813.898, 549.984, 129.301, 785.699, -902.723, 528.926, 554.156, 721.607, 359.952, -189.085, 956.156, -121.447, 679.843, -65.074, -137.243, -600.907, 86.4978, 733.596, -598.387, 155.462, 907.929, 17.2419, -973.024, -536.534, 171.033, -739.903, -134.797, -605.928, 883.188, 593.335, 264.864, -450.43, 713.097, 231.02, -955.378, -237.361, -286.716, 134.798, -573.354, -249.74, 779.743, 137.815, -976.079, 823.941, 467.053, -295.986, -727.813};
    std::vector<float> correct  {-976.079, -973.024, -955.378, -902.723, -813.898, -739.903, -727.813, -682.397, -605.928, -600.907, -598.387, -573.354, -536.534, -450.43, -446.874, -410.843, -295.986, -286.716, -249.74, -237.361, -205.312, -189.085, -174.688, -137.243, -134.797, -121.447, -103.326, -65.074, -60.4781, 17.2419, 86.4978, 129.301, 134.798, 137.815, 155.462, 162.026, 171.033, 224.369, 231.02, 264.864, 286.279, 335.916, 359.952, 362.071, 467.053, 528.926, 549.984, 554.156, 593.335, 668.151, 679.843, 711.33, 713.097, 721.607, 732.115, 733.596, 779.743, 785.699, 823.941, 883.188, 907.929, 944.08, 956.156, 993.383};

    auto res = app.bitonic_sort(sequence);
    EXPECT_EQ(1, std::get<0>(res) == correct);
}

TEST_F(bitonic_sort, accuracy_e20) {
    std::vector<float> sequence {2.0e-20, 5.0e-20, 6.0e-20, 1.0e-20, 4.0e-20, 3.0e-20};
    std::vector<float> correct  {1.0e-20, 2.0e-20, 3.0e-20, 4.0e-20, 5.0e-20, 6.0e-20};

    auto res = app.bitonic_sort(sequence);
    EXPECT_EQ(1, std::get<0>(res) == correct);
}

TEST_F(bitonic_sort, empty) {
    std::vector<float> sequence;
    std::vector<float> correct;

    auto res = app.bitonic_sort(sequence);
    EXPECT_EQ(1, std::get<0>(res) == correct);
}

} //GTests
