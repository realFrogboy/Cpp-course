#include <assert.h> 
#include <iostream>
#include <limits>
#include "tree_t.h"
#include "range_queries.h"
#include "input.h"

namespace range_queries {

void processing_tree::k_processing(const int val) {
    tree.rb_insert(tree.get_mgr()->create(val));
}

int processing_tree::q_processing(const int l_border, const int r_border) const {    
    int r_border_c = r_border + 1;  // algorithm count number of keys in [l_board, r_board) => 
                                    // if you whant find number of keys in [l_board, r_board] you should increment r_board

    return tree::tree_t::position(tree.get_root(), r_border_c) - tree::tree_t::position(tree.get_root(), l_border);
}

int processing_tree::n_processing(const int border) const {
    //border++;    algorithm count number of keys in (-oo, border) =>
                // if you whant find number of keys in (-oo, border] you should increment board 

    return tree::tree_t::position(tree.get_root(), border);
}

int processing_tree::m_processing(const int num) const {
    return tree.k_th_min(tree.get_root(), num);
}

std::vector<int> range_queries(const std::pair<std::vector<char>, std::vector<int>>& data) {
    processing_tree proc{};
    std::vector<int> res{};
    size_t idx = 0;

    for (char command : data.first) {
        switch (command) {
            case 'k':
                proc.k_processing(data.second[idx]);
                ++idx;
                break;
            
            case 'q': {
                int num = proc.q_processing(data.second[idx], data.second[idx + 1]);
                res.push_back(num);
                idx += 2;
                break;
            }
            
            case 'n': {
                int num = proc.n_processing(data.second[idx]);
                res.push_back(num);
                ++idx;
                break;
            }

            case 'm': {
                int key = proc.m_processing(data.second[idx]);
                res.push_back(key);
                ++idx;
            }
        }
    }

    #ifdef DUMP

    proc.dump();
    int chk = system("dot -Tpng tree_dump.dot -o image.png");
    if (chk < 0) 
        std::cout << "Can't drow tree" << std::endl;

    #endif

    return res;
}

} // range_queries