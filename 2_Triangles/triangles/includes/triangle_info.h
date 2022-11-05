#pragma once

#include "triangle_t.h"

namespace Triangles {

class triangle_info_t {
    public:
    Geometric::triangle_t triangle;
    unsigned number;
    bool is_intersect;
};

}