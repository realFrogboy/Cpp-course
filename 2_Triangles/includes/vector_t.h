#pragma once

#include <cmath>

namespace Triangles {

const double accuracy = 0.0001;

namespace Geometric {

struct coord_t {
    double x;
    double y;
    double z;
};

struct vector_t {

    coord_t coord {};

    vector_t(const double x = NAN, const double y = NAN, const double z = NAN) : coord{x, y, z} {};

    void copy(const vector_t& vec);

    bool isValid() const;

    bool isNull() const;

    bool isEqual(const vector_t& vec) const;

    bool isParallel(const vector_t& vec) const;

    vector_t normal1() const;

    vector_t normal2() const;

    vector_t normal3() const;

    double scalarMult(const vector_t& vec) const;

    vector_t vectorMult(const vector_t& vec) const;

    vector_t operator*(const double mul) const;

    vector_t operator+(vector_t vec);

    vector_t operator-(vector_t vec);

    double length() const;
};

inline void vector_t::copy(const vector_t& vec) {
    coord.x = vec.coord.x;
    coord.y = vec.coord.y;
    coord.z = vec.coord.z;
}

inline bool vector_t::isValid() const {
    if ((std::isfinite(coord.x)) && (std::isfinite(coord.y)) && (std::isfinite(coord.z)))
        return 1;
    return 0;
}

inline bool vector_t::isNull() const {
    if ((std::abs(coord.x) <= accuracy) && (std::abs(coord.y) <= accuracy) && (std::abs(coord.z) <= accuracy))
        return 0;
    return 1;
}

inline bool vector_t::isEqual(const vector_t& vec) const {
    if ((std::abs(coord.x - vec.coord.x) <= accuracy) && (std::abs(coord.y - vec.coord.y) <= accuracy) && (std::abs(coord.z - vec.coord.z) <= accuracy))
        return 1;
    return 0;
}

inline bool vector_t::isParallel(const vector_t& vec) const {
    double k = 0;
    if (vec.coord.x)
        k = coord.x / vec.coord.x;
    else if (vec.coord.y)
        k = coord.y / vec.coord.y;
    else
        k = coord.z / vec.coord.z;

    if ((std::abs(coord.x - vec.coord.x * k) <= accuracy) && (std::abs(coord.y - vec.coord.y * k) <= accuracy) && (std::abs(coord.z - vec.coord.z * k) <= accuracy))
        return 1;
    return 0;
}

inline vector_t vector_t::normal1() const {
    vector_t res(coord.y, -coord.x, 0);
    return res;
}

inline vector_t vector_t::normal2() const {
    vector_t res(coord.z, 0, -coord.x);
    return res;
}

inline vector_t vector_t::normal3() const {
    vector_t res(0, coord.z, -coord.y);
    return res;
}

inline double vector_t::scalarMult(const vector_t& vec) const { 
    return (coord.x * vec.coord.x + coord.y * vec.coord.y + coord.z * vec.coord.z); 
}

inline vector_t vector_t::vectorMult(const vector_t& vec) const {
    double x = coord.y * vec.coord.z - coord.z * vec.coord.y;
    double y = coord.z * vec.coord.x - coord.x * vec.coord.z;
    double z = coord.x * vec.coord.y - coord.y * vec.coord.x;

    vector_t res(x, y, z);
    return res;
}

inline vector_t vector_t::operator*(const double mul) const {
    double x = coord.x * mul;
    double y = coord.y * mul;
    double z = coord.z * mul;

    vector_t res(x, y, z);
    return res;
}

inline vector_t vector_t::operator+(vector_t vec) {
    double x = coord.x + vec.coord.x;
    double y = coord.y + vec.coord.y;
    double z = coord.z + vec.coord.z;

    vector_t res(x, y, z);
    return res;
}

inline vector_t vector_t::operator-(vector_t vec) {
    double x = coord.x - vec.coord.x;
    double y = coord.y - vec.coord.y;
    double z = coord.z - vec.coord.z;

    vector_t res(x, y, z);
    return res;
}

inline double vector_t::length() const{
    double tmp = scalarMult(*this);
    return sqrt(tmp);
}

} // Geometric
} // Triangles