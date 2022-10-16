#include "tree_t.h"
#include "node_t.h"
#include "range_queries.h"
#include "input.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <gtest/gtest.h>

namespace end_to_end {

TEST(range_queries, end_to_end1) {
    std::vector<int> commands{0, 10, 0, 20, 1, 8, 31, 1, 6, 9, 0, 30, 0, 40, 1, 15, 40, 2, 30, 3, 2, 1, 5, 50};
    std::vector<int> res = range_queries::range_queries(commands);
    std::vector<int> true_res = {2, 0, 3, 2, 20, 4};

    for (unsigned idx = 0; idx < res.size(); idx++)
        EXPECT_EQ(1, res[idx] == true_res[idx]);
}

TEST(range_queries, end_to_end2) {
    std::vector<int> commands{0, 3, 0, 7, 0, 11, 0, 10, 0, 18, 0, 22, 0, 26, 0, 8, 1, 8, 26, 1, 3, 7, 2, 18, 3, 8};
    std::vector<int> res = range_queries::range_queries(commands);
    std::vector<int> true_res = {6, 2, 5, 26};

    for (unsigned idx = 0; idx < res.size(); idx++)
        EXPECT_EQ(1, res[idx] == true_res[idx]);
}

TEST(range_queries, end_to_end3) {
    std::vector<int> commands{0, 7, 0, 6, 0, 3, 1, -1, 5, 2, 2, 0, 5, 0, 9, 0, 0, 1, 0, 10, 3, 1};
    std::vector<int> res = range_queries::range_queries(commands);
    std::vector<int> true_res = {1, 0, 6, 0};

    for (unsigned idx = 0; idx < res.size(); idx++)
        EXPECT_EQ(1, res[idx] == true_res[idx]);
}

} // end_to_end