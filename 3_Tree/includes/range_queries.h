#pragma once

#include <vector>
#include <utility>

namespace range_queries {

using tree::node_t;

std::vector<int> range_queries(const std::pair<std::vector<char>, std::vector<int>>& data);

class processing_tree {

    tree::tree_t tree;

    public:

    processing_tree() {};

    void k_processing(const int val);

    int q_processing(const int l_border, const int r_border) const;

    int n_processing(const int border) const;

    int m_processing(const int num) const;

    void dump() {
        tree.dump();
    }
};

} // range_queries