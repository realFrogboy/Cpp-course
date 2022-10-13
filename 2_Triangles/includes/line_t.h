#pragma once

#include "point_t.h"
#include <stdio.h>

namespace Geometric {

struct lineData_t {
    vector_t a;
    point_t linePt;
};

struct line_t {

    lineData_t lineData {};

    line_t(const vector_t dir = {NAN, NAN, NAN}, const point_t pt = {NAN, NAN, NAN}) : lineData{dir, pt} {};

    line_t(const point_t& firstPt, const point_t& secondPt);
    
    bool valid() const;

    bool onLine(const point_t& pt) const;

    double countD(const coord_t& coord1, const coord_t& coord2, const coord_t& coord3, const coord_t& coord4) const;

    double getT(const point_t& pt) const;

    point_t projOfPoint(const point_t& pt) const;

    point_t shortestLine(const line_t& line) const;
};

} // Geometric