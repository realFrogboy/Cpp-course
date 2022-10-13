#pragma once

#include <cmath>

namespace Triangles {

const double accurasy = 0.0001;

namespace Geometric {

extern const double accuracy;

struct coord_t {
    double x;
    double y;
    double z;
};

class vector_t {
    public:
    
    coord_t coord {};

    vector_t(const double x = NAN, const double y = NAN, const double z = NAN) : coord{x, y, z} {};

    inline void copy(const vector_t& vec);

    inline bool isValid() const;

    inline bool isNull() const;

    inline bool isEqual(const vector_t& vec) const;

    inline bool isParallel(const vector_t& vec) const;

    inline vector_t normal1() const;

    inline vector_t normal2() const;

    inline vector_t normal3() const;

    inline double scalarMult(const vector_t& vec) const;

    inline vector_t vectorMult(const vector_t& vec) const;

    inline vector_t operator*(const double mul) const;

    inline vector_t operator+(vector_t vec);

    inline vector_t operator-(vector_t vec);

    inline double length() const;
};

} // Geometric
