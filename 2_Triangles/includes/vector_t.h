#pragma once

#include <cmath>

namespace Geometric {

extern const double accuracy;

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

} // Geometric