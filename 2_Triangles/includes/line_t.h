#pragma once

#include "point_t.h"

namespace Triangles {
namespace Geometric {

struct lineData_t {
    vector_t a;
    point_t linePt;
};

class line_t {
    lineData_t lineData{{NAN, NAN, NAN}, {NAN, NAN, NAN}};

    public:

    inline line_t(vector_t dir, point_t pt) : lineData{dir, pt} {};

    inline line_t(const point_t& firstPt, const point_t& secondPt);

    inline lineData_t getData() const;

    inline bool valid() const;

    inline bool onLine(const point_t& pt) const;

    inline point_t isCross(const line_t& line) const;

    inline double getT(const point_t& pt) const;

    inline point_t projOfPoint(const point_t& pt) const;
};

inline line_t::line_t(const point_t& firstPt, const point_t& secondPt) : lineData{{NAN, NAN, NAN}, {NAN, NAN, NAN}} {
    vector_t firstVec  = firstPt.toVector();
    vector_t secondVec = secondPt.toVector();

    lineData.a.copy(secondVec - firstVec);
    lineData.linePt.copy(firstPt);
}

inline lineData_t line_t::getData() const { return lineData; }

inline bool line_t::valid() const {
    if ((lineData.a.isValid()) && (lineData.linePt.isValid()))
        return 1;
    return 0;
}

inline bool line_t::onLine(const point_t& pt) const {
    if (pt.isEqual(lineData.linePt))
        return 1;

    vector_t ptVec = pt.toVector();
    vector_t linePtVec = lineData.linePt.toVector();
    if (lineData.a.isParallel(linePtVec - ptVec))
        return 1;
    return 0;
}

inline point_t line_t::projOfPoint(const point_t& pt) const {
    vector_t ptVec = pt.toVector();
    vector_t linePtVec = lineData.linePt.toVector();

    double tmpCoeff = lineData.a.scalarMult(ptVec - linePtVec) / lineData.a.scalarMult(lineData.a);
    vector_t resVec = linePtVec + lineData.a * tmpCoeff;
    
    coord_t resCoord = resVec.getCoord();
    point_t resPt(resCoord.x, resCoord.y, resCoord.z);

    return resPt;
}

inline point_t line_t::isCross(const line_t& line) const {
    lineData_t lineData2 = line.getData();
    coord_t vecCoord1 = lineData.a.getCoord();
    coord_t vecCoord2 = lineData2.a.getCoord();
    coord_t ptCoord1  = lineData.linePt.getCoord();
    coord_t ptCoord2  = lineData2.linePt.getCoord();

    point_t invalidPt(NAN, NAN, NAN);

    double tmp1 = ptCoord1.y - ptCoord2.y + (ptCoord2.x - ptCoord1.x) * vecCoord1.y / vecCoord1.x;
    double tmp2 = (vecCoord2.y * vecCoord1.x - vecCoord2.x * vecCoord1.y) / vecCoord1.x;

    double t21 = tmp1 / tmp2;

    tmp1 = ptCoord1.z - ptCoord2.z + (ptCoord2.y - ptCoord1.y) * vecCoord1.z / vecCoord1.y;
    tmp2 = (vecCoord2.z * vecCoord1.y - vecCoord2.y * vecCoord1.z) / vecCoord1.y;

    double t22 = tmp1 / tmp2;
    
    if (std::abs(t21 - t22) > accurasy)
        return invalidPt;

    tmp1 = ptCoord1.x - ptCoord2.x + (ptCoord2.z - ptCoord1.z) * vecCoord1.x / vecCoord1.z;
    tmp2 = (vecCoord2.x * vecCoord1.z - vecCoord2.z * vecCoord1.x) / vecCoord1.z;

    double t23 = tmp1 / tmp2;

    if (std::abs(t22 - t23) > accurasy)
        return invalidPt;

    vector_t vecLinePt2 = lineData2.linePt.toVector();
    vector_t vecRes = vecLinePt2 + lineData2.a * t21;

    coord_t vecResCoord = vecRes.getCoord();
    point_t res(vecResCoord.x, vecResCoord.y, vecResCoord.z);
    return res;
}

inline double line_t::getT(const point_t& pt) const {
    vector_t ptVec = pt.toVector();
    vector_t linePtVec = lineData.linePt.toVector();

    vector_t tmp = ptVec - linePtVec;

    coord_t tmpCoord = tmp.getCoord();
    coord_t dirCoord = lineData.a.getCoord();

    double t = 0;
    if (dirCoord.x) 
        t = tmpCoord.x / dirCoord.x;
    else if (dirCoord.y)
        t = tmpCoord.y / dirCoord.y;
    else 
        t = tmpCoord.z / dirCoord.z;

    return t;
}

} // Geometric
} // Triangles