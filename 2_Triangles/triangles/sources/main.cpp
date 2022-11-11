#include "input.h"
#include "bsp.h"
#include "first_app.hpp"

#include <iostream>
#include <stdexcept>


int main(int argc, char ** argv) {
    std::vector<Triangles::triangle_info_t> triangles;
    Triangles::getData(triangles);

    bsp_tree::bspTree_t tree(triangles);
    tree.print(triangles);

    lve::FirstApp app{triangles};

    try {
        app.run(argc, argv);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}