#pragma once

#include <utility>
#include "assert.h"
#include "plate_t.h"

struct trianglePt_t {
    point_t first;
    point_t second;
    point_t third;
};

class triangle_t {
    trianglePt_t trianglePt{{NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}};

    inline bool isPoint() const;

    inline bool isLine() const;

    int checkSpecialCases(const triangle_t& triag) const;

    inline bool checkSign(const double& vertT, const line_t& normal, const point_t& vert) const;

    bool checkSepAxe(const line_t& edge, const triangle_t& triang, const plate_t& plt) const;

    int getDist(const double dist1, const double dist2, const double dist3) const;

    pair<double, double> getTT(const triangle_t& triag, const line_t& interLine, const int p) const;

    public:

    inline triangle_t(point_t pt1, point_t pt2, point_t pt3);

    inline triangle_t projOntoPlt(const plate_t& plt) const;

    bool isIntersection2D(const triangle_t& triang, const plate_t& plt) const;

    bool isIntersection3D(const triangle_t& triag) const;
};


inline bool triangle_t::isPoint() const {
    if (trianglePt.first.isEqual(trianglePt.second) && (trianglePt.second.isEqual(trianglePt.third)))
        return 1;
    return 0;
}

inline bool triangle_t::isLine() const {
    line_t line(trianglePt.first, trianglePt.second);
    bool res = line.onLine(trianglePt.third);
    return res;
}

inline bool triangle_t::checkSign(const double& vertT, const line_t& normal, const point_t& vert) const {
    point_t projVert = normal.projOfPoint(vert);
    double vert1T = normal.getT(projVert);
    
    if ((vertT * vert1T) >= 0)
        return 0;
    return 1;
}

inline triangle_t::triangle_t(point_t pt1, point_t pt2, point_t pt3) : trianglePt{{NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}} {
    trianglePt.first.copy(pt1);
    trianglePt.second.copy(pt2);
    trianglePt.third.copy(pt3);
}

inline triangle_t triangle_t::projOntoPlt(const plate_t& plt) const {
    point_t projFirst  = plt.projOfPoint(trianglePt.first);
    point_t projSecond = plt.projOfPoint(trianglePt.second);
    point_t projThird  = plt.projOfPoint(trianglePt.third);

    triangle_t triangle(projFirst, projSecond, projThird);
    return triangle;
}
