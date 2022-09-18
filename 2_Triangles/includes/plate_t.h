#pragma once

#include "line_t.h"

struct plateData_t {
    vector_t n;
    point_t platePt;
    double d;
};

class plate_t {
    plateData_t pltData {{NAN, NAN, NAN}, {NAN, NAN, NAN}, NAN};

    public:

    plate_t(const point_t& firstPt, const point_t& secondPt, const point_t& therdPt);

    inline plateData_t getData() const;

    inline bool valid() const ;

    inline bool isParallel(const plate_t& plt) const;

    inline bool isEqual(const plate_t& plt) const;

    inline double distToPoint(const point_t& pt) const;

    inline point_t projOfPoint(const point_t& pt) const;

    inline vector_t projOfVector( vector_t& vec) const;

    line_t intsectOf2Plt(const plate_t& plt) const;
};


inline plateData_t plate_t::getData() const { 
    return pltData; 
}

inline bool plate_t::valid() const {
    if ((pltData.n.isValid()) && (pltData.platePt.isValid()) && (isfinite(pltData.d)))
        return 1;
    return 0;
}

inline bool plate_t::isParallel(const plate_t& plt) const {
    if (pltData.n.isParallel(plt.pltData.n))
        return 1;
    return 0;
}

inline bool plate_t::isEqual(const plate_t& plt) const {
    if ((pltData.n.isParallel(plt.pltData.n)) && (abs(pltData.d - plt.pltData.d) <= accurasy))
        return 1;
    return 0;
}

inline double plate_t::distToPoint(const point_t& pt) const {
    vector_t ptVec = pt.toVector();
    double r = pltData.n.scalarMult(ptVec) + pltData.d;
    return r;
}

inline point_t plate_t::projOfPoint(const point_t& pt) const {
    plateData_t pltData = this->getData();
    vector_t thisVec = pt.toVector();

    vector_t projVec = thisVec - pltData.n * (thisVec.scalarMult(pltData.n) + pltData.d);
    coord_t vecCoord = projVec.getCoord();

    point_t projPt(vecCoord.x, vecCoord.y, vecCoord.z);
    return projPt;
}

inline vector_t plate_t::projOfVector( vector_t& vec) const {
    vector_t projVec = vec - pltData.n * (vec.scalarMult(pltData.n));
    return projVec;
}
