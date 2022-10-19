#include "tree_t.h"

namespace tree {

void tree_dump::graph_node(const node_t *node, std::ofstream& file) {
    if (node->l_subtree_size < 0)
        return;

    int curr = num;

    if (node->color == node_color::RED) {
        file << "\tnode" << num << " [shape = \"record\", style = \"filled\", fillcolor = \"red\", label = \"" << node->key << " : " << node->l_subtree_size << " : " << node->r_subtree_size << "\"];\n";
        num++;
    } else {
        file << "\tnode" << num << " [shape = \"record\", fontcolor = \"white\", style = \"filled\", fillcolor = \"grey28\", label = \"" << node->key << " : " << node->l_subtree_size << " : " << node->r_subtree_size << "\"];\n";
        num++;
    }

    graph_node(node->lhs, file);
    graph_node(node->rhs, file);

    if (!curr)
        num = 0;
    return;
}

void tree_dump::connect_node(const node_t *node, std::ofstream& file) {
    if ((node->lhs->l_subtree_size < 0) && (node->rhs->l_subtree_size < 0))
        return;

    int curr = num;

    if (node->lhs->l_subtree_size >= 0) {
        num++;
        file << "node" << curr << " -> node" << num << ";\n";

        connect_node(node->lhs, file);
    }

    if (node->rhs->l_subtree_size >= 0) {
        num++;
        file << "node" << curr << " -> node" << num << ";\n";

        connect_node(node->rhs, file);
    }
    if (!curr)
        num = 0;
    return;
}

} // tree