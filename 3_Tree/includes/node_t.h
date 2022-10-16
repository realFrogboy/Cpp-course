#pragma once

namespace tree_node {

enum class node_color {
    BLACK,
    RED
};

struct node_t {
    int key;
    int l_subtree_size = 0;
    int r_subtree_size = 0;
    node_color color = node_color::BLACK;
    node_t *lhs = nullptr;
    node_t *rhs = nullptr;
    node_t *parent = nullptr;

    node_t(const int k) : key(k) {};

    node_t(const node_t&) = delete;
    node_t &operator=(const node_t&) = delete;

    node_t(node_t&& rhs_) = delete;
    node_t &operator=(const node_t&&) = delete;
};

} // tree_node