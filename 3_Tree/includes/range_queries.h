#pragma once

#include <vector>

namespace range_queries {

using tree_node::node_t;

std::vector<int> range_queries(std::vector<int>& commands);

struct processing_tree {

    tree::tree_t tree;

    processing_tree() {};

    void k_processing(const int val);

    int q_processing(const int l_border, const int r_border) const;

    int n_processing(const int border) const;

    int m_processing(const int num) const;
};

} // range_queries