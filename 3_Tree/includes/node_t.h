#pragma once

namespace tree_node {

enum class node_color {
    BLACK,
    RED
};

struct node_t {
    int key;
    int subtree_size;
    node_color color = node_color::BLACK;
    node_t *lhs = nullptr;
    node_t *rhs = nullptr;
    node_t *parent = nullptr;

    node_t(const int k, const int sz = -1, const node_color cl = node_color::BLACK) : key(k), subtree_size(sz), color(cl) {};

    ~node_t() = default;

    node_t(const node_t&) = delete;
    node_t &operator=(const node_t&) = delete;

    node_t(const node_t&&) = delete;
    node_t &operator=(const node_t&&) = delete;
};

} // tree_node