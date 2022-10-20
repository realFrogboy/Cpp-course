#pragma once

#include <fstream>
#include <vector>

namespace tree {

enum class node_color {
    BLACK,
    RED
};

enum class side {
    RIGHT,
    LEFT
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

    int get_key() { return key; };

    node_t(const node_t&) = delete;
    node_t &operator=(const node_t&) = delete;

    node_t(node_t&& rhs_) = delete;
    node_t &operator=(const node_t&&) = delete;

    node_t **left () { return &lhs; }
    node_t **right() { return &rhs; }
};

class tree_dump {

    int num = 0;

    public:
    
    void graph_node(const node_t *node, std::ofstream& file);
    void connect_node(const node_t *node, std::ofstream& file);

};

class node_mgr {
    
    std::vector<node_t*> nodes;

    public:

    node_t *create(const int key); 

    void destruct(const node_t *node);

    ~node_mgr() {
        for (auto node : nodes)
            delete node;
    }
};

class tree_t {

    node_mgr mgr;

    node_t *root = nullptr;
    node_t *nil = nullptr;

    void rotate(node_t *node, const side side);

    void left_rotate(node_t *node);

    void right_rotate(node_t *node);

    void rb_insert_fixup(node_t *node);

    void rb_transplant(node_t *node_f, node_t *node_s);

    void rb_delete_fixup(node_t *node);

    public:

    tree_t() {}

    tree_t(node_t *node);

    node_t *get_root() const { return root; }

    node_t *get_nil() const { return nil; }

    node_mgr *get_mgr() { return &mgr; }

    tree_t(const tree_t& rhs) = delete;
    tree_t& operator=(const tree_t& rhs) = delete;

    tree_t(tree_t&& rhs) = delete;
    tree_t& operator=(tree_t&& rhs) = delete;

    node_t* tree_minimum(node_t *node) const;
    
    void rb_insert(node_t *node);
    
    void rb_delete(node_t *node);

    static int position(const node_t *node, const int key);

    int k_th_min(const node_t *node, const int num) const;

    void dump() const;
};

} // tree_t