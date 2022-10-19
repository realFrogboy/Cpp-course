#include <assert.h> 
#include <algorithm>
#include <iostream>
#include "tree_t.h"

namespace tree {

tree_t::tree_t(node_t *node) : root(node) {
    node_t *nil = mgr.create(0);

    nil->lhs = node;
    nil->rhs = node;

    node->lhs = nil;
    node->rhs = nil;
    node->parent = nil;
}

node_t* tree_t::tree_minimum(node_t *node) const {
    assert(node);

    node_t *copy = node;

    while (copy->lhs != nil)
        copy = copy->lhs;

    return copy;
}

void tree_t::rotate(node_t *node, const side side) {
    assert(node);

    node_t** (node_t::*p_lhs)();
    node_t** (node_t::*p_rhs)();

    if (side == side::LEFT) {
        p_lhs = &node_t::left;
        p_rhs = &node_t::right;
    } else {
        p_lhs = &node_t::right;
        p_rhs = &node_t::left;
    }

    assert(*(node->*p_rhs)() != nil);

    node_t *y = *(node->*p_rhs)();
    node_t **node_side = (node->*p_rhs)();
    *node_side = *(y->*p_lhs)();

    if (*(y->*p_lhs)() != nil)
        (*(y->*p_lhs)())->parent = node;

    y->parent = node->parent;

    if (node->parent == nil)
        root = y;
    else if (node == node->parent->lhs) 
        node->parent->lhs = y;
    else 
        node->parent->rhs = y;

    node_t **y_side = (y->*p_lhs)();
    *y_side = node;
    node->parent = y;

    if(side == side::LEFT) {
        node->r_subtree_size = y->l_subtree_size;
        y->l_subtree_size += node->l_subtree_size + 1;
    } else {
        node->l_subtree_size = y->r_subtree_size;
        y->r_subtree_size += node->r_subtree_size + 1;
    }
}

void tree_t::rb_insert(node_t *node) {
    assert(node);

    if (!root) {
        root = node;
        nil = mgr.create(0);

        nil->lhs = node;
        nil->rhs = node;
        nil->l_subtree_size = -1;
        nil->r_subtree_size = -1;

        node->lhs = nil;
        node->rhs = nil;
        node->parent = nil;
        root->color = node_color::BLACK;

        return;
    }

    node_t *y = nil;
    node_t *x = root;

    while (x != nil) {
        y = x;
        if (node->key < x->key) {
            x = x->lhs;
            y->l_subtree_size++;
        } else {
            x = x->rhs;
            y->r_subtree_size++;
        }
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
    node->color = node_color::RED;

    rb_insert_fixup(node);
}

void tree_t::rb_insert_fixup(node_t *node) {
    assert(node);

    while (node->parent->color == node_color::RED) {

        if (node->parent == node->parent->parent->rhs) {
            node_t *y = node->parent->parent->lhs;
            
            if (y->color == node_color::RED) {
                node->parent->color = node_color::BLACK;
                y->color = node_color::BLACK;
                node->parent->parent->color = node_color::RED;
                node = node->parent->parent;
            } else {
                
                if (node == node->parent->lhs) {
                    node = node->parent;
                    rotate(node, side::RIGHT);
                }

                node->parent->color = node_color::BLACK;
                node->parent->parent->color = node_color::RED;
                rotate(node->parent->parent, side::LEFT);
            }
        } else {
            node_t *y = node->parent->parent->rhs;
            
            if (y->color == node_color::RED) {
                node->parent->color = node_color::BLACK;
                y->color = node_color::BLACK;
                node->parent->parent->color = node_color::RED;
                node = node->parent->parent;
            } else {
                
                if (node == node->parent->rhs) {
                    node = node->parent;
                    rotate(node, side::LEFT);
                }

                node->parent->color = node_color::BLACK;
                node->parent->parent->color = node_color::RED;
                rotate(node->parent->parent, side::RIGHT);
            }
        }
    }
    root->color = node_color::BLACK;
}

void tree_t::rb_transplant(node_t *node_f, node_t *node_s) {
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

void tree_t::rb_delete(node_t *node) {
    assert(node);

    node_t *y = node;
    node_color y_original_color = y->color;
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

    if (y_original_color == node_color::BLACK)
        rb_delete_fixup(x);
    
    mgr.destruct(node);
}

void tree_t::rb_delete_fixup(node_t *node) {
    assert(node);

    while ((node != root) && (node->color == node_color::BLACK)) {
        if (node == node->parent->lhs) {
            node_t *w = node->parent->rhs;

            if(w->color == node_color::RED) {
                w->color = node_color::BLACK;
                node->parent->color = node_color::RED;
                rotate(node->parent, side::LEFT);
                w = node->parent->rhs;
            }

            if ((w->lhs->color == node_color::BLACK) && (w->rhs->color == node_color::BLACK)) {
                w->color = node_color::RED;
                node = node->parent;
            } else {
                if (w->rhs->color == node_color::BLACK) {
                    w->lhs->color = node_color::BLACK;
                    w->color = node_color::RED;
                    rotate(w, side::RIGHT);
                    w = node->parent->rhs;
                }

                w->color = node->parent->color;
                node->parent->color = node_color::BLACK;
                w->rhs->color = node_color::BLACK;
                rotate(node->parent, side::RIGHT);
                node = root;
            }
        } else {
            node_t *w = node->parent->lhs;

            if(w->color == node_color::RED) {
                w->color = node_color::BLACK;
                node->parent->color = node_color::RED;
                rotate(node->parent, side::RIGHT);
                w = node->parent->lhs;
            }

            if ((w->rhs->color == node_color::BLACK) && (w->lhs->color == node_color::BLACK)) {
                w->color = node_color::RED;
                node = node->parent;
            } else {
                if (w->lhs->color == node_color::BLACK) {
                    w->rhs->color = node_color::BLACK;
                    w->color = node_color::RED;
                    rotate(w, side::LEFT);
                    w = node->parent->lhs;
                }

                w->color = node->parent->color;
                node->parent->color = node_color::BLACK;
                w->lhs->color = node_color::BLACK;
                rotate(node->parent, side::RIGHT);
                node = root;
            }
        }
    }
    node->color = node_color::BLACK;
}

int tree_t::position(const node_t *node, const int key) {
    assert(node);

    if (node->l_subtree_size < 0)
        return 0;

    if (key < node->key)
        return position(node->lhs, key);
    else if (key > node->key) 
        return position(node->rhs, key) + node->l_subtree_size + 1;
    else
        return node->l_subtree_size;
}

int tree_t::k_th_min(const node_t *node, const int num) const {
    assert(node);

    if (node == nil)
        return 0;

    if (num <= node->l_subtree_size)
        return k_th_min(node->lhs, num);
    else if (num > node->l_subtree_size + 1) 
        return k_th_min(node->rhs, num - node->l_subtree_size - 1);
    else
        return node->key;
}

void tree_t::dump() const {
    std::ofstream file("tree_dump.dot");
    file << "digraph tree {\n";

    tree_dump dump{};
    dump.graph_node(root, file);
    dump.connect_node(root, file);

    file << "}";
    return;
}

node_t *node_mgr::create(const int key) {
    node_t *node = new node_t{key};
    nodes.push_back(node);
    return node;
}

void node_mgr::destruct(const node_t *node) {
    nodes.erase(std::remove_if(nodes.begin(), nodes.end(), [node](node_t *curr){ return curr == node; }), nodes.end());
    delete node;
}

} // tree