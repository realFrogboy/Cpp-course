#pragma once

#include "point_t.h"

struct lineData_t {
    vector_t a;
    point_t linePt;
};

class line_t {
    lineData_t lineData{{NAN, NAN, NAN}, {NAN, NAN, NAN}};

    public:

    inline line_t(vector_t dir, point_t pt);

    inline line_t(const point_t& firstPt, const point_t& secondPt);

    inline lineData_t getData() const;

    inline bool valid() const;

    inline bool onLine(const point_t& pt) const;

    point_t isCross(const line_t& line) const;

    double getT(const point_t& pt) const;

    inline point_t projOfPoint(const point_t& pt) const;
};


inline line_t::line_t(vector_t dir, point_t pt) : lineData{{NAN, NAN, NAN}, {NAN, NAN, NAN}} {
    lineData.a.copy(dir);
    lineData.linePt.copy(pt);
}

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
