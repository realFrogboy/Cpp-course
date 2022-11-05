#pragma once

#include "point_t.h"

namespace Triangles {
namespace Geometric {

struct lineData_t {
    vector_t a;
    point_t linePt;
};

struct line_t {

    lineData_t lineData {};

    line_t(const vector_t dir = {}, const point_t pt = {}) : lineData{dir, pt} {};

    line_t(const point_t& firstPt, const point_t& secondPt);
    
    bool valid() const;

    bool onLine(const point_t& pt) const;

    double countD(const coord_t& coord1, const coord_t& coord2, const coord_t& coord3, const coord_t& coord4) const;

    double getT(const point_t& pt) const;

    point_t projOfPoint(const point_t& pt) const;

    point_t shortestLine(const line_t& line) const;
};

inline bool line_t::valid() const {
    if ((lineData.a.isValid()) && (lineData.linePt.isValid()))
        return 1;
    return 0;
}

inline double line_t::countD(const coord_t& coord1, const coord_t& coord2, const coord_t& coord3, const coord_t& coord4) const {
    return  (coord1.x - coord2.x) * (coord4.x - coord3.x) + 
            (coord1.y - coord2.y) * (coord4.y - coord3.y) + 
            (coord1.z - coord2.z) * (coord4.z - coord3.z);
}

} // Geometric
} // Triangles