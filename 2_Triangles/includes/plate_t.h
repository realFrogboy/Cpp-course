#pragma once

#include "line_t.h"

namespace Geometric {

struct plateData_t {
    vector_t n;
    point_t platePt;
    double d;
};

struct plate_t {

    plateData_t pltData {};

    plate_t(const vector_t norm = {NAN, NAN, NAN}, const point_t pt = {NAN, NAN, NAN}, const double d = NAN) : pltData{norm, pt, d} {};

    plate_t(const point_t& firstPt, const point_t& secondPt, const point_t& thirdPt);

    bool valid() const ;

    bool isParallel(const plate_t& plt) const;

    bool isEqual(const plate_t& plt) const;

    double getCoeff(const plate_t& plt, const double s1, const double s2) const;

    double distToPoint(const point_t& pt) const;

    point_t projOfPoint(const point_t& pt) const;

    vector_t projOfVector( vector_t& vec) const;

    line_t intsectOf2Plt(const plate_t& plt) const;

    point_t intersectionLine(const line_t& line) const;
};

} // Geometric