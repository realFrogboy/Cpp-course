#pragma once

#include "node_t.h"
#include <fstream>
#include <assert.h> 
#include <list>

namespace tree {

using tree_node::node_t;

class tree_t { 

    inline void graph_node(const node_t *node, std::ofstream& file) const;

    inline void connect_node(const node_t *node, std::ofstream& file) const;

    public:

    node_t *root = nullptr;
    node_t *nil;

    inline tree_t(node_t *node);
    inline void left_rotate(node_t *node);
    inline void right_rotate(node_t *node);
    inline void rb_insert(node_t *node);
    inline void rb_insert_fixup(node_t *node);

    ~tree_t() { 
        if (root != nil) {
            delete root->lhs;
            delete root->rhs;
        }
    };

    inline void tree_dump() const;
};

inline tree_t::tree_t(node_t *node) : root(node), nil(new node_t{0, 0}) {
    nil->lhs = node;
    nil->rhs = node;
    node->lhs = nil;
    node->rhs = nil;
    node->parent = nil;
}

inline void tree_t::left_rotate(node_t *node) {
    assert(node);
    assert(node->rhs != nil);

    node_t *y = node->rhs;
    node->rhs = y->lhs;

    if (y->lhs != nil)
        y->lhs->parent = node;

    y->parent = node->parent;

    if (node->parent == nil)
        root = y;
    else if (node == node->parent->lhs) 
        node->parent->lhs = y;
    else 
        node->parent->rhs = y;

    y->lhs = node;
    node->parent = y;
}

inline void tree_t::right_rotate(node_t *node) {
    assert(node);
    assert(node->lhs != nil);

    node_t *y = node->lhs;
    node->lhs = y->rhs;

    if (y->rhs != nil)
        y->rhs->parent = node;

    y->parent = node->parent;

    if (node->parent == nil) 
        root = y;
    else if (node == node->parent->rhs)
        node->parent->rhs = y;
    else
        node->parent->lhs = y;

    y->rhs = node;
    node->parent = y;
}

inline void tree_t::rb_insert(node_t *node) {
    assert(node);

    node_t *y = nil;
    node_t *x = root;

    while (x != nil) {
        y = x;
        if (node->key < x->key)
            x = x->lhs;
        else 
            x = x->rhs;
    }

    node->parent = y;

    if (y == nil)
        root = node;
    else if (node->key < y->key)
        y->lhs = node;
    else    
        y->rhs = node;
    
    node->lhs = nil;
    node->rhs = nil;
    node->color = tree_node::node_color::RED;

    rb_insert_fixup(node);
}

inline void tree_t::rb_insert_fixup(node_t *node) {
    while (node->parent->color == tree_node::node_color::RED) {

        if (node->parent == node->parent->parent->rhs) {
            node_t *y = node->parent->parent->lhs;
            
            if (y->color == tree_node::node_color::RED) {
                node->parent->color = tree_node::node_color::BLACK;
                y->color = tree_node::node_color::BLACK;
                node->parent->parent->color = tree_node::node_color::RED;
                node = node->parent->parent;
            } else {
                
                if (node == node->parent->lhs) {
                    node = node->parent;
                    right_rotate(node);
                }

                node->parent->color = tree_node::node_color::BLACK;
                node->parent->parent->color = tree_node::node_color::RED;
                left_rotate(node->parent->parent);
            }
        } else {
            node_t *y = node->parent->parent->rhs;
            
            if (y->color == tree_node::node_color::RED) {
                node->parent->color = tree_node::node_color::BLACK;
                y->color = tree_node::node_color::BLACK;
                node->parent->parent->color = tree_node::node_color::RED;
                node = node->parent->parent;
            } else {
                
                if (node == node->parent->rhs) {
                    node = node->parent;
                    left_rotate(node);
                }

                node->parent->color = tree_node::node_color::BLACK;
                node->parent->parent->color = tree_node::node_color::RED;
                right_rotate(node->parent->parent);
            }
        }
    }
    root->color = tree_node::node_color::BLACK;
}

inline void tree_t::tree_dump() const {
    std::ofstream file("tree_dump.dot");
    file << "digraph tree {\n";
    graph_node(root, file);
    connect_node(root, file);
    file << "}";
    return;
}

inline void tree_t::graph_node(const node_t *node, std::ofstream& file) const {
    if (node == nil)
        return;

    static int num = 0;
    int curr = num;

    if (node->color == tree_node::node_color::RED) {
        file << "\tnode" << num << " [shape = \"record\", style = \"filled\", fillcolor = \"red\", label = \"" << node->key << "\"];\n";
        num++;
    } else {
        file << "\tnode" << num << " [shape = \"record\", style = \"filled\", fillcolor = \"grey28\", label = \"" << node->key << "\"];\n";
        num++;
    }

    graph_node(node->lhs, file);
    graph_node(node->rhs, file);

    if (!curr)
        num = 0;
    return;
}

inline void tree_t::connect_node(const node_t *node, std::ofstream& file) const {
    if ((node->lhs == nil) && (node->rhs == nil))
        return;

    static int num = 0;
    int curr = num;

    if (node->lhs != nil) {
        num++;
        file << "node" << curr << " -> node" << num << ";\n";

        connect_node(node->lhs, file);
    }

    if (node->rhs != nil) {
        num++;
        file << "node" << curr << " -> node" << num << ";\n";

        connect_node(node->rhs, file);
    }
    if (!curr)
        num = 0;
    return;
}

}