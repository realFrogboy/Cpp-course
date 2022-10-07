#pragma once

#include "node_t.h"
#include <fstream>
#include <assert.h> 
#include <algorithm>
#include <list>

namespace tree {

using tree_node::node_t;

namespace tree_dump {

    inline void graph_node(const node_t *nil, const node_t *node, std::ofstream& file);

    inline void connect_node(const node_t *nil, const node_t *node, std::ofstream& file);

} // tree_dump

class tree_t {

    inline void left_rotate(node_t *node);

    inline void right_rotate(node_t *node);

    inline void rb_insert_fixup(node_t *node);

    inline void rb_transplant(node_t *node_f, node_t *node_s);

    inline void rb_delete_fixup(node_t *node);

    public:

    std::list<node_t*> nodes;

    node_t *root = nullptr;
    node_t *nil;

    inline tree_t(node_t *node);

    inline node_t* tree_minimum(node_t *node) const;
    
    inline void rb_insert(node_t *node);
    
    inline void rb_delete(node_t *node);

    inline int inorder_pos(const node_t *node) const;

    inline int position(const node_t *node, const int key) const;

    inline int k_th_min(const node_t *node, const int num) const;

    inline void dump() const;

    ~tree_t() { 
        for (auto node : nodes)
            delete node;
    }
};

inline tree_t::tree_t(node_t *node) : root(node), nil(new node_t{0, 0}) {
    nil->lhs = node;
    nil->rhs = node;

    node->lhs = nil;
    node->rhs = nil;
    node->parent = nil;

    nodes.push_back(root);
    nodes.push_back(nil);
}

inline node_t* tree_t::tree_minimum(node_t *node) const {
    assert(node);

    node_t *copy = node;

    while (copy->lhs != nil)
        copy = copy->lhs;

    return copy;
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
    
    nodes.push_back(node);
}

inline void tree_t::rb_insert_fixup(node_t *node) {
    assert(node);

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

inline void tree_t::rb_transplant(node_t *node_f, node_t *node_s) {
    assert(node_f);
    assert(node_s);

    if (node_f->parent == nil)
        root = node_s;
    else if (node_f == node_f->parent->lhs)
        node_f->parent->lhs = node_s;
    else
        node_f->parent->rhs = node_s;
    
    node_s->parent = node_f->parent;
}

inline void tree_t::rb_delete(node_t *node) {
    assert(node);

    node_t *y = node;
    tree_node::node_color y_original_color = y->color;
    node_t *x = nullptr;

    if (node->lhs == nil) {
        x = node->rhs;
        rb_transplant(node, node->rhs);
    } else if (node->rhs == nil) {
        x = node->lhs;
        rb_transplant(node, node->lhs);
    } else {
        y = tree_minimum(node->rhs);
        y_original_color = y->color;
        x = y->rhs;

        if (y->parent == node) 
            x->parent = y;
        else {
            rb_transplant(y, y->rhs);
            y->rhs = node->rhs;
            y->rhs->parent = y;
        }

        rb_transplant(node, y);
        y->lhs = node->lhs;
        y->lhs->parent = y;
        y->color = node->color;
    }

    if (y_original_color == tree_node::node_color::BLACK)
        rb_delete_fixup(x);
    
    auto nodeIter = std::find(nodes.begin(), nodes.end(), node);
    nodes.erase(nodeIter);
    delete node;
}

inline void tree_t::rb_delete_fixup(node_t *node) {
    assert(node);

    while ((node != root) && (node->color == tree_node::node_color::BLACK)) {
        if (node == node->parent->lhs) {
            node_t *w = node->parent->rhs;

            if(w->color == tree_node::node_color::RED) {
                w->color = tree_node::node_color::BLACK;
                node->parent->color = tree_node::node_color::RED;
                left_rotate(node->parent);
                w = node->parent->rhs;
            }

            if ((w->lhs->color == tree_node::node_color::BLACK) && (w->rhs->color == tree_node::node_color::BLACK)) {
                w->color = tree_node::node_color::RED;
                node = node->parent;
            } else {
                if (w->rhs->color == tree_node::node_color::BLACK) {
                    w->lhs->color = tree_node::node_color::BLACK;
                    w->color = tree_node::node_color::RED;
                    right_rotate(w);
                    w = node->parent->rhs;
                }

                w->color = node->parent->color;
                node->parent->color = tree_node::node_color::BLACK;
                w->rhs->color = tree_node::node_color::BLACK;
                left_rotate(node->parent);
                node = root;
            }
        } else {
            node_t *w = node->parent->lhs;

            if(w->color == tree_node::node_color::RED) {
                w->color = tree_node::node_color::BLACK;
                node->parent->color = tree_node::node_color::RED;
                right_rotate(node->parent);
                w = node->parent->lhs;
            }

            if ((w->rhs->color == tree_node::node_color::BLACK) && (w->lhs->color == tree_node::node_color::BLACK)) {
                w->color = tree_node::node_color::RED;
                node = node->parent;
            } else {
                if (w->lhs->color == tree_node::node_color::BLACK) {
                    w->rhs->color = tree_node::node_color::BLACK;
                    w->color = tree_node::node_color::RED;
                    left_rotate(w);
                    w = node->parent->lhs;
                }

                w->color = node->parent->color;
                node->parent->color = tree_node::node_color::BLACK;
                w->lhs->color = tree_node::node_color::BLACK;
                right_rotate(node->parent);
                node = root;
            }
        }
    }
    node->color = tree_node::node_color::BLACK;
}

inline int tree_t::inorder_pos(const node_t *node) const {
    assert(node);

    if (node == nil)
        return 0;

    int cnt = inorder_pos(node->lhs);
    cnt++;
    cnt += inorder_pos(node->rhs);

    return cnt;
}

inline int tree_t::position(const node_t *node, const int key) const {
    assert(node);

    if (node == nil)
        return 0;

    if (key < node->key)
        return position(node->lhs, key);
    else if (key > node->key) 
        return position(node->rhs, key) + inorder_pos(node->lhs) + 1;
    else
        return inorder_pos(node->lhs);
}

inline int tree_t::k_th_min(const node_t *node, const int num) const {
    assert(node);

    if (node == nil)
        return 0;

    if (num <= inorder_pos(node->lhs))
        return k_th_min(node->lhs, num);
    else if (num > inorder_pos(node->lhs) + 1) 
        return k_th_min(node->rhs, num - inorder_pos(node->lhs) - 1);
    else
        return node->key;
}

inline void tree_t::dump() const {
    std::ofstream file("tree_dump.dot");
    file << "digraph tree {\n";
    tree_dump::graph_node(nil, root, file);
    tree_dump::connect_node(nil, root, file);
    file << "}";
    return;
}

inline void tree_dump::graph_node(const node_t *nil, const node_t *node, std::ofstream& file) {
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

    graph_node(nil, node->lhs, file);
    graph_node(nil, node->rhs, file);

    if (!curr)
        num = 0;
    return;
}

inline void tree_dump::connect_node(const node_t *nil, const node_t *node, std::ofstream& file) {
    if ((node->lhs == nil) && (node->rhs == nil))
        return;

    static int num = 0;
    int curr = num;

    if (node->lhs != nil) {
        num++;
        file << "node" << curr << " -> node" << num << ";\n";

        connect_node(nil, node->lhs, file);
    }

    if (node->rhs != nil) {
        num++;
        file << "node" << curr << " -> node" << num << ";\n";

        connect_node(nil, node->rhs, file);
    }
    if (!curr)
        num = 0;
    return;
}

} // tree_t