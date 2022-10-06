#include "bsp.h"

namespace bsp_tree {

location classifyTriangle(const plate_t& plt, const triangle_t& triag) {
    Triangles::Geometric::trianglePt_t trianglePt = triag.trianglePt;

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

std::pair<triangle_t, unsigned> getSplit(const nodeVec& arr) {
    for (auto triag : arr) {
        if ((!triag.first.isPoint()) && (!triag.first.isLine()))
            return triag;
    }

    triangle_t invalid{};
    return std::make_pair(invalid, 0u);
}

} // bsp_tree