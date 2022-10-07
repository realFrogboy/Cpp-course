#include "range_queries.h"

namespace range_queries {

using tree_node::node_t;

namespace {

    tree::tree_t initialize_tree() {
        char command;
        int val = 0;
        assert(std::cin >> command >> val);
        assert(command == 'k');

        node_t *root  = new node_t{val};
        tree::tree_t tree{root};

        return tree;
    }

    void k_processing(tree::tree_t& tree) {
        int val = 0;
        assert(std::cin >> val);

        node_t *node = new node_t{val};
        tree.rb_insert(node);
    }

    int q_processing(const tree::tree_t& tree) {
        int l_border = 0, r_border = 0;
        assert(std::cin >> l_border >> r_border);
        
        r_border++; // algorithm count number of keys in [l_board, r_board) => 
                    // if you whant find number of keys in [l_board, r_board] you should increment r_board

        return tree.position(tree.root, r_border) - tree.position(tree.root, l_border);
    }

    int n_processing(const tree::tree_t& tree) {
        int border = 0;
        assert(std::cin >> border);

        //border++;    algorithm count number of keys in (-oo, border) =>
                    // if you whant find number of keys in (-oo, border] you should increment board 

        return tree.position(tree.root, border);
    }

    int m_processing(const tree::tree_t& tree) {
        int num = 0;
        assert(std::cin >> num);

        return tree.k_th_min(tree.root, num);
    }

}

std::vector<int> range_queries() {
    tree::tree_t tree = initialize_tree();

    char command;
    assert(std::cin >> command);
    std::vector<int> res{};
    while ((command == 'k') || (command == 'q') || (command == 'n') || (command == 'm')) {
        switch (command) {
            case 'k':
                k_processing(tree);
                assert(std::cin >> command);
                break;
            
            case 'q': {
                int num = q_processing(tree);
                res.push_back(num);
                assert(std::cin >> command);
                break;
            }
            
            case 'n': {
                int num = n_processing(tree);
                res.push_back(num);
                assert(std::cin >> command);
                break;
            }

            case 'm': {
                int key = m_processing(tree);
                res.push_back(key);
                assert(std::cin >> command);
            }
        }
    }

    tree.dump();
    system("dot -Tpng tree_dump.dot -o image.png");

    return res;
}

} // range_queries