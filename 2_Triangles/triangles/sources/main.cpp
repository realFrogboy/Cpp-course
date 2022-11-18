#include "input.h"
#include "bsp.h"
#include "first_app.hpp"
#include "help.hpp"

#include <iostream>
#include <stdexcept>


int main(int argc, char ** argv) {
    bool is_help = lve::help(argc, argv);
    if (is_help) return 0;

    std::vector<Triangles::triangle_info_t> triangles;
    Triangles::getData(triangles);

    bsp_tree::bspTree_t tree(triangles);
    tree.print(triangles);

    lve::FirstApp app{triangles};

    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}