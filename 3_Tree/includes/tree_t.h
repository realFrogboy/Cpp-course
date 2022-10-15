#pragma once

#include "node_t.h"
#include <fstream>
#include <vector>

namespace tree {

using tree_node::node_t;

int position(const node_t *nil, const node_t *node, const int key);

enum class side {
    RIGHT,
    LEFT
};

struct tree_dump {

    int num = 0;
    
    void graph_node(const node_t *nil, const node_t *node, std::ofstream& file);
    void connect_node(const node_t *nil, const node_t *node, std::ofstream& file);

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

    std::vector<node_t*> nodes;

    node_t *root = nullptr;
    node_t *nil = nullptr;

    void left_rotate(node_t *node);

    void right_rotate(node_t *node);

    void rb_insert_fixup(node_t *node);

    void rb_transplant(node_t *node_f, node_t *node_s);

    void rb_delete_fixup(node_t *node);

    public:

    tree_t() {};

    tree_t(node_t *node);

    node_t *get_root() const;

    node_t *get_nil() const;

    tree_t(tree_t&& rhs);

    tree_t& operator=(tree_t&& rhs);

    node_t* tree_minimum(node_t *node) const;
    
    void rb_insert(node_t *node);
    
    void rb_delete(node_t *node);

    int k_th_min(const node_t *node, const int num) const;

    void dump() const;

    /*~tree_t() { 
        for (auto node : nodes)
            delete node;
    }*/
};

} // tree_t