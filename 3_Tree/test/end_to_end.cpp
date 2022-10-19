#include "tree_t.h"
#include "range_queries.h"
#include "input.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <gtest/gtest.h>

namespace end_to_end {

TEST(range_queries, end_to_end1) {
    std::vector<char> commands{'k', 'k', 'q', 'q', 'k', 'k', 'q', 'n', 'm', 'q'};
    std::vector<int> numbers{10, 20, 8, 31, 6, 9, 30, 40, 15, 40, 30, 2, 5, 50};
    std::vector<int> res = range_queries::range_queries(std::pair{commands, numbers});
    std::vector<int> true_res = {2, 0, 3, 2, 20, 4};

    for (unsigned idx = 0; idx < res.size(); idx++)
        EXPECT_EQ(1, res[idx] == true_res[idx]);
}

TEST(range_queries, end_to_end2) {
    std::vector<char> commands{'k','k', 'k', 'k', 'k', 'k', 'k', 'k', 'q', 'q', 'n', 'm'};
    std::vector<int> numbers{3, 7, 11, 10, 18, 22, 26, 8, 8, 26, 3, 7, 18, 8};
    std::vector<int> res = range_queries::range_queries(std::pair{commands, numbers});
    std::vector<int> true_res = {6, 2, 5, 26};

    for (unsigned idx = 0; idx < res.size(); idx++)
        EXPECT_EQ(1, res[idx] == true_res[idx]);
}

TEST(range_queries, end_to_end3) {
    std::vector<char> commands{'k', 'k', 'k', 'q', 'n', 'k', 'k', 'k', 'q', 'm'};
    std::vector<int> numbers{7, 6, 3, -1, 5, 2, 5, 9, 0, 0, 10, 1};
    std::vector<int> res = range_queries::range_queries(std::pair{commands, numbers});
    std::vector<int> true_res = {1, 0, 6, 0};

    for (unsigned idx = 0; idx < res.size(); idx++)
        EXPECT_EQ(1, res[idx] == true_res[idx]);
}

} // end_to_end