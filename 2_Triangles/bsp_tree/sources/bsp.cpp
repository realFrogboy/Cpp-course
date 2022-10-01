#include "bsp.h"

namespace bsp_tree {

void toArr(std::pair<triangle_type, unsigned>* arr, const nodeVec& vector) {
    for (auto elem : vector) {
        arr->second = elem.second;
        arr->first = elem.first;
        arr++;
    }
}

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

std::pair<triangle_type, unsigned> getSplit(const std::pair<triangle_type, unsigned>* arr, int n) {
    for (int idx = 0; idx < n; idx++) {
        if ((!arr[idx].first.isPoint()) && (!arr[idx].first.isLine()))
            return arr[idx];
    }

    triangle_type invalid{};
    return std::make_pair(invalid, 0u);
}

} // bsp_tree