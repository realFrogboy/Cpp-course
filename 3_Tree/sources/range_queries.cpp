#include <assert.h> 
#include <iostream>
#include <limits>
#include "tree_t.h"
#include "node_t.h"
#include "range_queries.h"
#include "input.h"

namespace range_queries {

void processing_tree::k_processing(const int val) {
    tree.rb_insert(tree.get_mgr()->create(val));
}

int processing_tree::q_processing(const int l_border,const int r_border) const {    
    int r_border_c = r_border + 1;  // algorithm count number of keys in [l_board, r_board) => 
                                    // if you whant find number of keys in [l_board, r_board] you should increment r_board

    return tree::position(tree.get_root(), r_border_c) - tree::position(tree.get_root(), l_border);
}

int processing_tree::n_processing(const int border) const {
    //border++;    algorithm count number of keys in (-oo, border) =>
                // if you whant find number of keys in (-oo, border] you should increment board 

    return tree::position(tree.get_root(), border);
}

int processing_tree::m_processing(const int num) const {
    return tree.k_th_min(tree.get_root(), num);
}

std::vector<int> range_queries(std::vector<int>& commands) {
    processing_tree proc{};

    std::vector<int> res{};
    unsigned idx = 0, sz = commands.size();
    while (idx < sz) {
        switch (commands[idx]) {
            case static_cast<int>(get_commands::command_codes::K):
                proc.k_processing(commands[idx + 1]);
                idx += 2;
                break;
            
            case static_cast<int>(get_commands::command_codes::Q): {
                int num = proc.q_processing(commands[idx + 1], commands[idx + 2]);
                res.push_back(num);
                idx += 3;
                break;
            }
            
            case static_cast<int>(get_commands::command_codes::N): {
                int num = proc.n_processing(commands[idx + 1]);
                res.push_back(num);
                idx += 2;
                break;
            }

            case static_cast<int>(get_commands::command_codes::M): {
                int key = proc.m_processing(commands[idx + 1]);
                res.push_back(key);
                idx += 2;
            }
        }
    }

    #ifdef DUMP

    proc.tree.dump();
    int chk = system("dot -Tpng tree_dump.dot -o image.png");
    if (chk < 0) 
        std::cout << "Can't drow tree" << std::endl;

    #endif

    return res;
}

} // range_queries