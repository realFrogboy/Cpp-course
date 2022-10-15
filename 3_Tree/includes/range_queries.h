#pragma once

#include <vector>

namespace range_queries {

using tree_node::node_t;

std::vector<int> range_queries();

struct processing_tree {

    tree::tree_t tree;

    processing_tree();

    void k_processing();

    int q_processing() const;

    int n_processing() const;

    int m_processing() const;
};

} // range_queries