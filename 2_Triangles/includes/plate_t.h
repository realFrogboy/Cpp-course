#pragma once

#include "line_t.h"

namespace Triangles {
namespace Geometric {

struct plateData_t {
    vector_t n;
    point_t platePt;
    double d;
};

class plate_t {
    plateData_t pltData {{NAN, NAN, NAN}, {NAN, NAN, NAN}, NAN};

    public:

    inline plate_t(const point_t& firstPt, const point_t& secondPt, const point_t& therdPt);

    inline plateData_t getData() const;

    inline bool valid() const ;

    inline bool isParallel(const plate_t& plt) const;

    inline bool isEqual(const plate_t& plt) const;

    inline double distToPoint(const point_t& pt) const;

    inline point_t projOfPoint(const point_t& pt) const;

    inline vector_t projOfVector( vector_t& vec) const;

    inline line_t intsectOf2Plt(const plate_t& plt) const;

    inline point_t intersectionLine(const line_t& line) const;
};


inline plateData_t plate_t::getData() const { 
    return pltData; 
}

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
    if ((pltData.n.isParallel(plt.pltData.n)) && (std::abs(pltData.d - plt.pltData.d) <= accurasy))
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

inline plate_t::plate_t(const point_t& firstPt, const point_t& secondPt, const point_t& therdPt) : pltData {{NAN, NAN, NAN}, {NAN, NAN, NAN}, NAN} {
    vector_t firstVec  = firstPt.toVector();
    vector_t secondVec = secondPt.toVector();
    vector_t therdVec  = therdPt.toVector();

    vector_t norm = (secondVec - firstVec).vectorMult(therdVec - firstVec);

    coord_t normCoord = norm.getCoord();
    double normalCoeff = sqrt(normCoord.x * normCoord.x + normCoord.y * normCoord.y + normCoord.z * normCoord.z);

    vector_t normNorm = norm * (1/normalCoeff);

    pltData.n.copy(normNorm);
    pltData.platePt.copy(firstPt);
    pltData.d = -firstVec.scalarMult(pltData.n);
}

inline line_t plate_t::intsectOf2Plt(const plate_t& plt) const {
    // parallel or equal plates
    if(this->isParallel(plt)) {
        vector_t a{NAN, NAN, NAN};
        point_t pt{NAN, NAN, NAN};
        line_t res(a, pt);
        return res;
    }

    // not parallel or equal plates
    vector_t aVec = pltData.n.vectorMult(plt.pltData.n);
    double s1 = -pltData.d;
    double s2 = -plt.pltData.d;

    double a = (s2 * pltData.n.scalarMult(plt.pltData.n) - s1 * plt.pltData.n.scalarMult(plt.pltData.n)) / 
                (pltData.n.scalarMult(plt.pltData.n) * pltData.n.scalarMult(plt.pltData.n) - pltData.n.scalarMult(pltData.n) * plt.pltData.n.scalarMult(plt.pltData.n));
    double b = (s1 * pltData.n.scalarMult(plt.pltData.n) - s2 * pltData.n.scalarMult(pltData.n)) / 
                (pltData.n.scalarMult(plt.pltData.n) * pltData.n.scalarMult(plt.pltData.n) - pltData.n.scalarMult(pltData.n) * plt.pltData.n.scalarMult(plt.pltData.n));

    vector_t commVec = pltData.n * a + plt.pltData.n * b;
    coord_t coords = commVec.getCoord();
    point_t commPt(coords.x, coords.y, coords.z);

    line_t line(aVec, commPt);
    return line;
}

inline point_t plate_t::intersectionLine(const line_t& line) const {
    lineData_t lineData = line.getData();
    vector_t linePtVec = lineData.linePt.toVector();

    double t = -(pltData.n.scalarMult(linePtVec) + pltData.d) / pltData.n.scalarMult(lineData.a);

    vector_t resVec = linePtVec + lineData.a * t;
    coord_t resCoord = resVec.getCoord();
    
    point_t pt(resCoord.x, resCoord.y, resCoord.z);
    return pt;
}

} // Geometric
} // Triangles