#include "point_t.h"

namespace Geometric {

point_t point_t::getMinX(const point_t& pt) const {
    if (coord.x > pt.coord.x)
        return *this;
    return pt;
}

point_t point_t::getMinY(const point_t& pt) const {
    if (coord.y > pt.coord.y)
        return *this;
    return pt;
}

point_t point_t::getMinZ(const point_t& pt) const {
    if (coord.z > pt.coord.z)
        return *this;
    return pt;
}

void point_t::copy(const point_t& pt) {
    coord.x = pt.coord.x;
    coord.y = pt.coord.y;
    coord.z = pt.coord.z;
}

bool point_t::isValid() const {
    if ((std::isfinite(coord.x)) && (std::isfinite(coord.y)) && (std::isfinite(coord.z)))
        return 1;
    return 0;
}

bool point_t::isNull() const {
    if ((std::abs(coord.x) <= accuracy) && (std::abs(coord.y) <= accuracy) && (std::abs(coord.z) <= accuracy))
        return 0;
    return 1;
}

bool point_t::isEqual(const point_t& pt) const {
    if ((std::abs(coord.x - pt.coord.x) <= accuracy) && (std::abs(coord.y - pt.coord.y) <= accuracy) && (std::abs(coord.z - pt.coord.z) <= accuracy))
        return 1;
    return 0;
}

vector_t point_t::toVector() const {
    vector_t vec(coord.x, coord.y, coord.z);
    return vec;
}

} // Geomrtric