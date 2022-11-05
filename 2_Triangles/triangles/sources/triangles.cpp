#include "triangles.h"

namespace Triangles {

namespace {

Geometric::point_t getPoint() {
    double x = 0, y = 0, z = 0;
    assert(std::cin >> x >> y >> z);

    Geometric::point_t pt(x, y, z);
    return pt;
}

} //namespace

void getData(std::vector<triangle_info_t>& triangles) {
    unsigned n = 0;
    std::cin >> n;
    assert(std::cin.good());

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