#include "line_t.h"

namespace Geometric {

line_t::line_t(const point_t& firstPt, const point_t& secondPt) {
    vector_t firstVec  = firstPt.toVector();
    vector_t secondVec = secondPt.toVector();

    lineData.a.copy(secondVec - firstVec);
    lineData.linePt.copy(firstPt);
}

bool line_t::valid() const {
    if ((lineData.a.isValid()) && (lineData.linePt.isValid()))
        return 1;
    return 0;
}

bool line_t::onLine(const point_t& pt) const {
    if (pt.isEqual(lineData.linePt))
        return 1;

    vector_t ptVec = pt.toVector();
    vector_t linePtVec = lineData.linePt.toVector();
    if (lineData.a.isParallel(linePtVec - ptVec))
        return 1;
    return 0;
}

point_t line_t::projOfPoint(const point_t& pt) const {
    vector_t ptVec = pt.toVector();
    vector_t linePtVec = lineData.linePt.toVector();

    double tmpCoeff = lineData.a.scalarMult(ptVec - linePtVec) / lineData.a.scalarMult(lineData.a);
    vector_t resVec = linePtVec + lineData.a * tmpCoeff;
    
    coord_t resCoord = resVec.coord;
    point_t resPt(resCoord.x, resCoord.y, resCoord.z);

    return resPt;
}

double line_t::getT(const point_t& pt) const {
    vector_t ptVec = pt.toVector();
    vector_t linePtVec = lineData.linePt.toVector();

    vector_t tmp = ptVec - linePtVec;

    coord_t tmpCoord = tmp.coord;
    coord_t dirCoord = lineData.a.coord;

    double t = 0;
    if (dirCoord.x) 
        t = tmpCoord.x / dirCoord.x;
    else if (dirCoord.y)
        t = tmpCoord.y / dirCoord.y;
    else 
        t = tmpCoord.z / dirCoord.z;

    return t;
}

point_t line_t::shortestLine(const line_t& line) const {
    if (lineData.a.isParallel(line.lineData.a) && (line.onLine(lineData.linePt)))
        return lineData.linePt;

    vector_t linePtVec1 = lineData.linePt.toVector();
    vector_t linePtVec3 = line.lineData.linePt.toVector();

    vector_t linePtVec2 = linePtVec1 + lineData.a;
    vector_t linePtVec4 = linePtVec3 + line.lineData.a;

    coord_t coord1 = linePtVec1.coord;
    coord_t coord2 = linePtVec2.coord;
    coord_t coord3 = linePtVec3.coord;
    coord_t coord4 = linePtVec4.coord;

    double d1343 = countD(coord1, coord3, coord4, coord3);
    double d4321 = countD(coord4, coord3, coord2, coord1);
    double d1321 = countD(coord1, coord3, coord2, coord1);
    double d4343 = countD(coord4, coord3, coord4, coord3);
    double d2121 = countD(coord2, coord1, coord2, coord1);
    
    double t1 = (d1343 * d4321 - d1321 * d4343) / (d2121 * d4343 - d4321 * d4321);
    double t2 = (d1343 + t1 * d4321) / d4343;

    vector_t pt1 = linePtVec1 + lineData.a * t1;
    vector_t pt2 = linePtVec3 + line.lineData.a * t2;

    vector_t lenVec = pt1 - pt2;
    double len = lenVec.length();

    if (std::abs(len) < accuracy) {
        coord_t resCoord = pt1.coord;
        point_t resPt {resCoord.x, resCoord.y, resCoord.z};
        return resPt;
    } else {
        point_t invalidPt {};
        return invalidPt;
    }
}

double line_t::countD(const coord_t& coord1, const coord_t& coord2, const coord_t& coord3, const coord_t& coord4) const {
    return  (coord1.x - coord2.x) * (coord4.x - coord3.x) + 
            (coord1.y - coord2.y) * (coord4.y - coord3.y) + 
            (coord1.z - coord2.z) * (coord4.z - coord3.z);
}

} // Geometric