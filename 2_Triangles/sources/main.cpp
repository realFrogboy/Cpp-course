#include "triangles.h"
#include "bsp.h"

int main() {
    std::list<std::pair<Triangles::Geometric::triangle_t, unsigned>> triangles;
    Triangles::getData(triangles);

    bsp_tree::bspTree_t tree(triangles);
    tree.inorder(tree.root);
    tree.deleteTree();

    return 0;
}