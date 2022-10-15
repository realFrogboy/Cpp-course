#include "tree_t.h"

namespace tree {

using tree_node::node_t;

void tree_dump::graph_node(const node_t *nil, const node_t *node, std::ofstream& file) {
    if (node == nil)
        return;

    int curr = num;

    if (node->color == tree_node::node_color::RED) {
        file << "\tnode" << num << " [shape = \"record\", style = \"filled\", fillcolor = \"red\", label = \"" << node->key << " : " << node->l_subtree_size << " : " << node->r_subtree_size << "\"];\n";
        num++;
    } else {
        file << "\tnode" << num << " [shape = \"record\", fontcolor = \"white\", style = \"filled\", fillcolor = \"grey28\", label = \"" << node->key << " : " << node->l_subtree_size << " : " << node->r_subtree_size << "\"];\n";
        num++;
    }

    graph_node(nil, node->lhs, file);
    graph_node(nil, node->rhs, file);

    if (!curr)
        num = 0;
    return;
}

void tree_dump::connect_node(const node_t *nil, const node_t *node, std::ofstream& file) {
    if ((node->lhs == nil) && (node->rhs == nil))
        return;

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

} // tree