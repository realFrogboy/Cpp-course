#pragma once

#include "triangle_t.h"

namespace bounding_box {

using Triangles::Geometric::point_t;
using Triangles::Geometric::triangle_t;
using Triangles::Geometric::coord_t;

point_t getMin(const triangle_t& triag);
point_t getMax(const triangle_t& triag);

class AABB {
 
    point_t min;
    point_t max;

    public:

    AABB(const triangle_t& triag);

    bool isIntersect(const AABB& box) const;
};

inline AABB::AABB(const triangle_t& triag) {
    max = getMax(triag);
    min = getMin(triag);
}

inline bool AABB::isIntersect(const AABB& box) const {
    coord_t coordMax1 = max.coord;
    coord_t coordMin1 = min.coord;
    coord_t coordMax2 = box.max.coord;
    coord_t coordMin2 = box.min.coord;

    if (coordMax1.x < coordMin2.x || coordMin1.x > coordMax2.x) return false;
    if (coordMax1.y < coordMin2.y || coordMin1.y > coordMax2.y) return false;
    if (coordMax1.z < coordMin2.z || coordMin1.z > coordMax2.z) return false;
    
    return true;
}

} // bounding_box