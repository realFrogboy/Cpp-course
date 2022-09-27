#pragma once

#include "point_t.h"
#include <stdio.h>

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

    inline double getT(const point_t& pt) const;

    inline point_t projOfPoint(const point_t& pt) const;

    inline point_t shortestLine(const line_t& line) const;
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

inline point_t line_t::shortestLine(const line_t& line) const {
    if (lineData.a.isParallel(line.lineData.a) && (line.onLine(lineData.linePt)))
        return lineData.linePt;

    vector_t linePtVec1 = lineData.linePt.toVector();
    vector_t linePtVec3 = line.lineData.linePt.toVector();

    vector_t linePtVec2 = linePtVec1 + lineData.a;
    vector_t linePtVec4 = linePtVec3 + line.lineData.a;

    coord_t coord1 = linePtVec1.getCoord();
    coord_t coord2 = linePtVec2.getCoord();
    coord_t coord3 = linePtVec3.getCoord();
    coord_t coord4 = linePtVec4.getCoord();

    double d1343 = (coord1.x - coord3.x) * (coord4.x - coord3.x) + (coord1.y - coord3.y) * (coord4.y - coord3.y) + (coord1.z - coord3.z) * (coord4.z - coord3.z);
    double d4321 = (coord4.x - coord3.x) * (coord2.x - coord1.x) + (coord4.y - coord3.y) * (coord2.y - coord1.y) + (coord4.z - coord3.z) * (coord2.z - coord1.z);
    double d1321 = (coord1.x - coord3.x) * (coord2.x - coord1.x) + (coord1.y - coord3.y) * (coord2.y - coord1.y) + (coord1.z - coord3.z) * (coord2.z - coord1.z);
    double d4343 = (coord4.x - coord3.x) * (coord4.x - coord3.x) + (coord4.y - coord3.y) * (coord4.y - coord3.y) + (coord4.z - coord3.z) * (coord4.z - coord3.z);
    double d2121 = (coord2.x - coord1.x) * (coord2.x - coord1.x) + (coord2.y - coord1.y) * (coord2.y - coord1.y) + (coord2.z - coord1.z) * (coord2.z - coord1.z);
    
    double t1 = (d1343 * d4321 - d1321 * d4343) / (d2121 * d4343 - d4321 * d4321);
    double t2 = (d1343 + t1 * d4321) / d4343;

    vector_t pt1 = linePtVec1 + lineData.a * t1;
    vector_t pt2 = linePtVec3 + line.lineData.a * t2;

    vector_t lenVec = pt1 - pt2;
    double len = lenVec.length();

    if (std::abs(len) < accurasy) {
        coord_t resCoord = pt1.getCoord();
        point_t resPt {resCoord.x, resCoord.y, resCoord.z};
        return resPt;
    } else {
        point_t invalidPt {NAN, NAN, NAN};
        return invalidPt;
    }
}

} // Geometric
} // Triangles