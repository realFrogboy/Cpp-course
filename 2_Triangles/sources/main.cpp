#include "triangles.h"
#include "bsp.h"


int main() {
    std::vector<std::pair<Geometric::triangle_t, unsigned>> triangles;
    Triangles::getData(triangles);

    bsp_tree::bspTree_t tree(triangles);
    tree.print();

    return 0;
}