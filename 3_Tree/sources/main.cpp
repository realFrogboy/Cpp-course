#include "node_t.h"
#include "tree_t.h"
#include <iostream>

namespace {
using tree_node::node_t;
}

int main() {
    node_t nd{3};

    tree::tree_t tree(&nd);

    for (int i = 0; i <= 100; i += 10) {
        node_t *node = new node_t{i};
        tree.rb_insert(node);
    }

    for (int i = 99; i >= 1; i -= 10) {
        node_t *node = new node_t{i};
        tree.rb_insert(node);
    }

    for (int i = 2; i >= 98; i += 10) {
        node_t *node = new node_t{i};
        tree.rb_insert(node);
    }

    for (int i = 97; i >= 3; i -= 10) {
        node_t *node = new node_t{i};
        tree.rb_insert(node);
    }

    for (int i = 4; i >= 96; i += 10) {
        node_t *node = new node_t{i};
        tree.rb_insert(node);
    }
    
    for (int i = 95; i >= 5; i -= 10) {
        node_t *node = new node_t{i};
        tree.rb_insert(node);
    }

    tree.tree_dump();
    system("dot -Tpng tree_dump.dot -o image.png");

    std::cout << tree.root->lhs->key << std::endl;
}