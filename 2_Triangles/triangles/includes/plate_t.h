#pragma once

#include "line_t.h"

namespace Triangles {
namespace Geometric {

struct plateData_t {
    vector_t n;
    point_t platePt;
    double d;
};

struct plate_t {

    plateData_t pltData {};

    plate_t(const vector_t norm = {}, const point_t pt = {}, const double d = NAN) : pltData{norm, pt, d} {};

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

inline bool plate_t::valid() const {
    if ((pltData.n.isValid()) && (pltData.platePt.isValid()) && (std::isfinite(pltData.d)))
        return 1;
    return 0;
}

inline bool plate_t::isParallel(const plate_t& plt) const {
    if (pltData.n.isParallel(plt.pltData.n))
        return 1;
    return 0;
}

inline bool plate_t::isEqual(const plate_t& plt) const {
    if ((pltData.n.isParallel(plt.pltData.n)) && (std::abs(pltData.d - plt.pltData.d) <= accuracy))
        return 1;
    return 0;
}

inline double plate_t::distToPoint(const point_t& pt) const {
    vector_t ptVec = pt.toVector();
    double r = pltData.n.scalarMult(ptVec) + pltData.d;
    return r;
}

inline vector_t plate_t::projOfVector( vector_t& vec) const {
    vector_t projVec = vec - pltData.n * (vec.scalarMult(pltData.n));
    return projVec;
}

inline double plate_t::getCoeff(const plate_t& plt, const double s1, const double s2) const {
    return  (s2 * pltData.n.scalarMult(plt.pltData.n) - s1 * plt.pltData.n.scalarMult(plt.pltData.n)) / 
            (pltData.n.scalarMult(plt.pltData.n) * pltData.n.scalarMult(plt.pltData.n) - 
                pltData.n.scalarMult(pltData.n) * plt.pltData.n.scalarMult(plt.pltData.n));
}

} // Geometric
} // Triangles