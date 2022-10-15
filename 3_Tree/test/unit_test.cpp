#include "tree_t.h"
#include "range_queries.h"
#include <gtest/gtest.h>

namespace tree_unit_test {

using tree_node::node_t;

class rb_tree_test : public ::testing::Test {
    protected:
    void SetUp() override {
        tree.rb_insert(root);
        tree.rb_insert(node5);
        tree.rb_insert(node3);
        tree.rb_insert(node6);
        tree.rb_insert(node2);
        tree.rb_insert(node4);
        tree.rb_insert(node7);
    } 

    node_t *root = new node_t{7};
    node_t *node1 = new node_t{3};
    node_t *node2 = new node_t{18};
    node_t *node3 = new node_t{10};
    node_t *node4 = new node_t{22};
    node_t *node5 = new node_t{8};
    node_t *node6 = new node_t{11};
    node_t *node7 = new node_t{26};
    tree::tree_t tree{node1};
};

TEST_F(rb_tree_test, tree_minimum) {
    node_t *res = tree.tree_minimum(tree.get_root());
    EXPECT_EQ(1, res->key == 3);

    res = tree.tree_minimum(node1);
    EXPECT_EQ(1, res->key == 3);

    res = tree.tree_minimum(node2);
    EXPECT_EQ(1, res->key == 11);

    res = tree.tree_minimum(node4);
    EXPECT_EQ(1, res->key == 22);
}

TEST_F(rb_tree_test, position) {
    int res = tree::position(tree.get_nil(), tree.get_root(), 2);
    EXPECT_EQ(1, res == 0);

    res = tree::position(tree.get_nil(), tree.get_root(), 3);
    EXPECT_EQ(1, res == 0);

    res = tree::position(tree.get_nil(), tree.get_root(), 4);
    EXPECT_EQ(1, res == 1);

    res = tree::position(tree.get_nil(), tree.get_root(), 5);
    EXPECT_EQ(1, res == 1);

    res = tree::position(tree.get_nil(), tree.get_root(), 7);
    EXPECT_EQ(1, res == 1);

    res = tree::position(tree.get_nil(), tree.get_root(), 8);
    EXPECT_EQ(1, res == 2);

    res = tree::position(tree.get_nil(), tree.get_root(), 9);
    EXPECT_EQ(1, res == 3);

    res = tree::position(tree.get_nil(), tree.get_root(), 10);
    EXPECT_EQ(1, res == 3);

    res = tree::position(tree.get_nil(), tree.get_root(), 11);

    EXPECT_EQ(1, res == 4);

    res = tree::position(tree.get_nil(), tree.get_root(), 12);
    EXPECT_EQ(1, res == 5);

    res = tree::position(tree.get_nil(), tree.get_root(), 18);
    EXPECT_EQ(1, res == 5);

    res = tree::position(tree.get_nil(), tree.get_root(), 19);
    EXPECT_EQ(1, res == 6);

    res = tree::position(tree.get_nil(), tree.get_root(), 22);
    EXPECT_EQ(1, res == 6);

    res = tree::position(tree.get_nil(), tree.get_root(), 23);
    EXPECT_EQ(1, res == 7);

    res = tree::position(tree.get_nil(), tree.get_root(), 26);
    EXPECT_EQ(1, res == 7);

    res = tree::position(tree.get_nil(), tree.get_root(), 27);
    EXPECT_EQ(1, res == 8);
}

TEST_F(rb_tree_test, k_th_min) {
    int res = tree.k_th_min(tree.get_root(), 1);
    EXPECT_EQ(1, res == 3);
    
    res = tree.k_th_min(tree.get_root(), 2);
    EXPECT_EQ(1, res == 7);

    res = tree.k_th_min(tree.get_root(), 3);
    EXPECT_EQ(1, res == 8);

    res = tree.k_th_min(tree.get_root(), 4);
    EXPECT_EQ(1, res == 10);

    res = tree.k_th_min(tree.get_root(), 5);
    EXPECT_EQ(1, res == 11);

    res = tree.k_th_min(tree.get_root(), 6);
    EXPECT_EQ(1, res == 18);

    res = tree.k_th_min(tree.get_root(), 7);
    EXPECT_EQ(1, res == 22);

    res = tree.k_th_min(tree.get_root(), 8);
    EXPECT_EQ(1, res == 26);
}

} // tree_unit_test