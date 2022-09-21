#pragma once

#include "vector_t.h"

namespace Triangles {

namespace Geometric {

class point_t {
    coord_t coord{NAN, NAN, NAN};

    public:

    point_t() {};

    point_t(double x, double y, double z) : coord{x, y, z} {};

    inline coord_t getCoord() const;

    inline void copy(const point_t& pt);

    inline bool isValid() const;

    inline bool isNull() const;

    inline bool isEqual(const point_t& pt) const;

    inline vector_t toVector() const;
};


inline coord_t point_t::getCoord() const { 
    return coord; 
}

inline void point_t::copy(const point_t& pt) {
    coord.x = pt.coord.x;
    coord.y = pt.coord.y;
    coord.z = pt.coord.z;
}

inline bool point_t::isValid() const {
    if ((std::isfinite(coord.x)) && (std::isfinite(coord.y)) && (std::isfinite(coord.z)))
        return 1;
    return 0;
}

inline bool point_t::isNull() const {
    if ((std::abs(coord.x) <= accurasy) && (std::abs(coord.y) <= accurasy) && (std::abs(coord.z) <= accurasy))
        return 0;
    return 1;
}

inline bool point_t::isEqual(const point_t& pt) const {
    if ((std::abs(coord.x - pt.coord.x) <= accurasy) && (std::abs(coord.y - pt.coord.y) <= accurasy) && (std::abs(coord.z - pt.coord.z) <= accurasy))
        return 1;
    return 0;
}

inline vector_t point_t::toVector() const {
    vector_t vec(coord.x, coord.y, coord.z);
    return vec;
}

} // Geometric
} // Trinagles