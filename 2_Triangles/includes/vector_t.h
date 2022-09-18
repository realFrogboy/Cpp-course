#pragma once

#include <cmath>

using namespace std;

const double accurasy = 0.01;

struct coord_t {
    double x;
    double y;
    double z;
};

class vector_t {
    coord_t coord{NAN, NAN, NAN};

    public:

    vector_t(double x, double y, double z) : coord{x, y, z} {};

    inline coord_t getCoord() const;

    inline void copy(const vector_t& vec);

    inline bool isValid() const;

    inline bool isNull() const;

    inline bool isEqual(const vector_t& vec) const;

    inline bool isParallel(const vector_t& vec) const;

    inline vector_t normal1() const;

    inline vector_t normal2() const;

    inline double scalarMult(const vector_t& vec) const;

    inline vector_t vectorMult(const vector_t& vec) const;

    inline vector_t operator*(const double mul) const;

    inline vector_t operator+(vector_t vec);

    inline vector_t operator-(vector_t vec);

    inline double length() const;
};


inline coord_t vector_t::getCoord() const { 
    return coord; 
}

inline void vector_t::copy(const vector_t& vec) {
    coord.x = vec.coord.x;
    coord.y = vec.coord.y;
    coord.z = vec.coord.z;
}

inline bool vector_t::isValid() const {
    if ((isfinite(coord.x)) && (isfinite(coord.y)) && (isfinite(coord.z)))
        return 1;
    return 0;
}

inline bool vector_t::isNull() const {
    if ((abs(coord.x) <= accurasy) && (abs(coord.y) <= accurasy) && (abs(coord.z) <= accurasy))
        return 0;
    return 1;
}

inline bool vector_t::isEqual(const vector_t& vec) const {
    if ((abs(coord.x - vec.coord.x) <= accurasy) && (abs(coord.y - vec.coord.y) <= accurasy) && (abs(coord.z - vec.coord.z) <= accurasy))
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

    if ((abs(coord.x - vec.coord.x * k) <= accurasy) && (abs(coord.y - vec.coord.y * k) <= accurasy) && abs(coord.z - vec.coord.z * k) <= accurasy)
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
    double tmp =  this->scalarMult(*this);
    return sqrt(tmp);
}
