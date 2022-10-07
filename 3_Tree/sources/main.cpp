#include "node_t.h"
#include "tree_t.h"
#include "range_queries.h"
#include <iostream>

namespace {
using tree_node::node_t;
}

int main() {
    std::vector<int> res = range_queries::range_queries();
    for (auto num : res)
        std::cout << num << std::endl;
}