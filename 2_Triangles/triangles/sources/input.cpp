#include "input.h"

#include <iostream>
#include <limits>

namespace {

void cls() {
    std::cout << "Incorrect input" << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

template <typename T>
T get() {
    T val;
    std::cin >> val;

    if (std::cin.eof()) {
        throw std::runtime_error("not enough data to create a triangle");
    }

    while((!std::cin) || (!std::isspace(std::cin.peek()))) {
        cls();
        std::cin >> val;
    }
    return val;
}

} // namespace

namespace Triangles {

namespace {

Geometric::point_t getPoint() {
    double x = get<double>(), y = get<double>(), z = get<double>();
    Geometric::point_t pt(x, y, z);
    return pt;
}

} //namespace

void getData(std::vector<triangle_info_t>& triangles) {
    unsigned n = get<unsigned>();

    for (unsigned cnt = 0; cnt < n; cnt++) {
        Triangles::Geometric::point_t pt1 = Triangles::getPoint();
        Triangles::Geometric::point_t pt2 = Triangles::getPoint();
        Triangles::Geometric::point_t pt3 = Triangles::getPoint();

        Triangles::Geometric::triangle_t triag(pt1, pt2, pt3);

        triangle_info_t info{{pt1, pt2, pt3}, cnt, 0};

        triangles.push_back(info);
    }
}

} // Triangles