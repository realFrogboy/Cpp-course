#pragma once

#include "vector_t.h"

namespace Geometric {

struct point_t {

    coord_t coord {};

    point_t(const double x = NAN, const double y = NAN, const double z = NAN) : coord{x, y, z} {};

    point_t getMinX(const point_t& pt) const;
    point_t getMinY(const point_t& pt) const;
    point_t getMinZ(const point_t& pt) const;

    void copy(const point_t& pt);

    bool isValid() const;

    bool isNull() const;

    bool isEqual(const point_t& pt) const;

    vector_t toVector() const;
};

} // Geometric