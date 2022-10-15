#include <assert.h> 
#include <iostream>
#include <limits>
#include "tree_t.h"
#include "node_t.h"
#include "range_queries.h"

namespace {
    void cls() {
        std::cout << "Incorrect input" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

namespace range_queries {

processing_tree::processing_tree() {
    char command = 0;
    int val = 0;

    std::cin >> command >> val;
    if (!std::cin) {
        cls();
        std::cin >> command >> val;
    }

    //if (command == 'k')

    node_t *root  = new node_t{val};
    tree::tree_t tmp{root};
    tree = std::move(tmp);
}

void processing_tree::k_processing() {
    int val = 0;
    std::cin >> val;
    if (!std::cin) {
        cls();
        std::cout << "Enter unsigned" << std::endl;
        std::cin >> val;
    }

    node_t *node = new node_t{val};
    tree.rb_insert(node);
}

int processing_tree::q_processing() const {
    int l_border = 0, r_border = 0;
    std::cin >> l_border >> r_border;
    if(!std::cin) {
        cls();
        std::cout << "Enter 2 unsigned" << std::endl;
        std::cin >> l_border >> r_border;
    }
    
    r_border++; // algorithm count number of keys in [l_board, r_board) => 
                // if you whant find number of keys in [l_board, r_board] you should increment r_board

    return tree::position(tree.get_nil(), tree.get_root(), r_border) - tree::position(tree.get_nil(), tree.get_root(), l_border);
}

int processing_tree::n_processing() const {
    int border = 0;
    std::cin >> border;
    if(!std::cin) {
        cls();
        std::cout << "Enter unsigned" << std::endl;
        std::cin >> border;
    }

    //border++;    algorithm count number of keys in (-oo, border) =>
                // if you whant find number of keys in (-oo, border] you should increment board 

    return tree::position(tree.get_nil(), tree.get_root(), border);
}

int processing_tree::m_processing() const {
    int num = 0;
    std::cin >> num;
    if (!std::cin) {
        cls();
        std::cout << "Enter unsigned" << std::endl;
        std::cin >> num;
    }

    return tree.k_th_min(tree.get_root(), num);
}

std::vector<int> range_queries() {
    processing_tree proc{};

    char command;
    std::cin >> command;
    if(!std::cin) {
        cls();
        std::cin >> command;
    }
    std::vector<int> res{};
    while (((command == 'k') || (command == 'q') || (command == 'n') || (command == 'm')) && (!std::cin.eof())) {
        switch (command) {
            case 'k':
                proc.k_processing();
                std::cin >> command;
                break;
            
            case 'q': {
                int num = proc.q_processing();
                res.push_back(num);
                std::cin >> command;
                break;
            }
            
            case 'n': {
                int num = proc.n_processing();
                res.push_back(num);
                std::cin >> command;
                break;
            }

            case 'm': {
                int key = proc.m_processing();
                res.push_back(key);
                std::cin >> command;
            }
        }
    }

    #ifdef DUMP

    proc.tree.dump();
    system("dot -Tpng tree_dump.dot -o image.png");

    #endif

    return res;
}

} // range_queries