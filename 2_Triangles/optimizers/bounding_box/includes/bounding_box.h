#pragma once

#include "triangle_t.h"

namespace bounding_box {

using Geometric::point_t;
using Geometric::triangle_t;
using Geometric::coord_t;

point_t getMin(const triangle_t& triag);
point_t getMax(const triangle_t& triag);

class AABB {
 
    point_t min;
    point_t max;

    public:

    AABB(const triangle_t& triag);

    bool isIntersect(const AABB& box) const;
};

} // bounding_box