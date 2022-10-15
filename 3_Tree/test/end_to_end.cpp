#include "tree_t.h"
#include "node_t.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <gtest/gtest.h>

namespace end_to_end {

using tree_node::node_t;

namespace {

    tree::tree_t initialize_tree(std::ifstream& file) {
        char command;
        int val = 0;
        file >> command >> val;

        node_t *root  = new node_t{val};
        tree::tree_t tree{root};

        return tree;
    }

    void k_processing(tree::tree_t& tree, std::ifstream& file) {
        int val = 0;
        file >> val;

        node_t *node = new node_t{val};
        tree.rb_insert(node);
    }

    int q_processing(const tree::tree_t& tree, std::ifstream& file) {
        int l_border = 0, r_border = 0;
        file >> l_border >> r_border;
        
        r_border++; // algorithm count number of keys in [l_board, r_board) => 
                    // if you whant find number of keys in [l_board, r_board] you should increment r_board

        return tree::position(tree.get_nil(), tree.get_root(), r_border) - tree::position(tree.get_nil(), tree.get_root(), l_border);
    }

    int n_processing(const tree::tree_t& tree, std::ifstream& file) {
        int border = 0;
        file >> border;

        //border++;    algorithm count number of keys in (-oo, border) =>
                    // if you whant find number of keys in (-oo, border] you should increment board 

        return tree::position(tree.get_nil(), tree.get_root(), border);
    }

    int m_processing(const tree::tree_t& tree, std::ifstream& file) {
        int num = 0;
        file >> num;

        return tree.k_th_min(tree.get_root(), num);
    }

}

std::vector<int> range_queries(std::ifstream& file) {
    tree::tree_t tree = initialize_tree(file);

    char command;
    assert(file >> command);
    std::vector<int> res{};
    while ((command == 'k') || (command == 'q') || (command == 'n') || (command == 'm')) {
        switch (command) {
            case 'k':
                k_processing(tree, file);
                file >> command;
                break;
            
            case 'q': {
                int num = q_processing(tree, file);
                res.push_back(num);
                file >> command;
                break;
            }
            
            case 'n': {
                int num = n_processing(tree, file);
                res.push_back(num);
                file >> command;
                break;
            }

            case 'm': {
                int key = m_processing(tree, file);
                res.push_back(key);
                file >> command;
            }
        }
    }
    return res;
}

TEST(range_queries, end_to_end1) {
    std::ifstream file1("test1.txt");
    assert(file1.is_open());
    std::vector<int> res = range_queries(file1);
    std::vector<int> true_res = {2, 0, 3, 2, 20, 4};

    for (unsigned idx = 0; idx < res.size(); idx++)
        EXPECT_EQ(1, res[idx] == true_res[idx]);
}

TEST(range_queries, end_to_end2) {
    std::ifstream file2("test2.txt");
    assert(file2.is_open());
    std::vector<int> res = range_queries(file2);
    std::vector<int> true_res = {6, 2, 5, 26};

    for (unsigned idx = 0; idx < res.size(); idx++)
        EXPECT_EQ(1, res[idx] == true_res[idx]);
}

TEST(range_queries, end_to_end3) {
    std::ifstream file3("test3.txt");
    assert(file3.is_open());
    std::vector<int> res = range_queries(file3);
    std::vector<int> true_res = {1, 0, 6, 0};

    for (unsigned idx = 0; idx < res.size(); idx++)
        EXPECT_EQ(1, res[idx] == true_res[idx]);
}

} // end_to_end