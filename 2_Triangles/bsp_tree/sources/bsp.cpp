#include "bsp.h"

namespace bsp_tree {

location classifyTriangle(const Triangles::Geometric::plate_t& plt, const Triangles::Geometric::triangle_t& triag) {
    Triangles::Geometric::trianglePt_t trianglePt = triag.getData();

    double dist1 = plt.distToPoint(trianglePt.first);
    double dist2 = plt.distToPoint(trianglePt.second);
    double dist3 = plt.distToPoint(trianglePt.third);

    if ((std::abs(dist1) < accurasy) && (std::abs(dist2) < accurasy) && (std::abs(dist3) < accurasy))
        return location::COINCIDENT;
    else if ((dist1 > 0) && (dist2 > 0) && (dist3 > 0))
        return location::FRONT;
    else if ((dist1 < 0) && (dist2 < 0) && (dist3 < 0)) 
        return location::BACK;
    else
        return location::INTERSECTION;
}

std::pair<Triangles::Geometric::triangle_t, unsigned> getSplit(const std::list<std::pair<Triangles::Geometric::triangle_t, unsigned>>& triangles) {
    auto iter = triangles.begin();
    for (; iter != triangles.end(); ++iter) {
        if ((!iter->first.isPoint()) && (!iter->first.isLine()))
            break;
    }

    if (iter == triangles.end()) {
        Triangles::Geometric::triangle_t invalid{};
        return std::make_pair(invalid, 0u);
    }

    return *iter;
}

} // bsp_tree