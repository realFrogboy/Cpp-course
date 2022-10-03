#pragma once

#include "triangle_t.h"

namespace bounding_box {

Triangles::Geometric::point_t getMin(const Triangles::Geometric::triangle_t& triag);
Triangles::Geometric::point_t getMax(const Triangles::Geometric::triangle_t& triag);

class AABB {
    public:

    Triangles::Geometric::point_t min;
    Triangles::Geometric::point_t max;

    AABB(const Triangles::Geometric::triangle_t& triag);

    bool isIntersect(const AABB& box) const;
};

inline AABB::AABB(const Triangles::Geometric::triangle_t& triag) {
    max = getMax(triag);
    min = getMin(triag);
}

inline bool AABB::isIntersect(const AABB& box) const {
    Triangles::Geometric::coord_t coordMax1 = max.getCoord();
    Triangles::Geometric::coord_t coordMin1 = min.getCoord();
    Triangles::Geometric::coord_t coordMax2 = box.max.getCoord();
    Triangles::Geometric::coord_t coordMin2 = box.min.getCoord();

    if (coordMax1.x < coordMin2.x || coordMin1.x > coordMax2.x) return false;
    if (coordMax1.y < coordMin2.y || coordMin1.y > coordMax2.y) return false;
    if (coordMax1.z < coordMin2.z || coordMin1.z > coordMax2.z) return false;
    
    return true;
}

} // bounding_box