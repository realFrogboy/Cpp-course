#pragma once

#include <utility>
#include <limits.h>
#include "assert.h"
#include "plate_t.h"

namespace Geometric {

struct trianglePt_t {
    point_t first;
    point_t second;
    point_t third;
};

struct  triangle_t {

    trianglePt_t trianglePt {};

    triangle_t(const point_t pt1 = {NAN, NAN, NAN}, const point_t pt2 = {NAN, NAN, NAN}, const point_t pt3 = {NAN, NAN, NAN}) : trianglePt{pt1, pt2, pt3} {};

    bool operator==(const triangle_t& triag) const;

    std::pair<point_t, point_t> getMinMax() const; 

    bool isPoint() const;

    bool isLine() const;

    int checkSpecialCases(const triangle_t& triag) const;

    bool checkSign(const double& vertT, const line_t& normal, const point_t& vert) const;

    bool checkSepAxe(const line_t& edge, const triangle_t& triang, const plate_t& plt) const;

    int getDist(const double dist1, const double dist2, const double dist3) const;

    std::pair<double, double> getTT(const triangle_t& triag, const line_t& interLine, const int p) const;

    triangle_t projOntoPlt(const plate_t& plt) const;

    bool isIntersection2D(const triangle_t& triang, const plate_t& plt) const;

    bool isIntersectionWithPoint(const triangle_t triag) const;

    bool isIntersectionLinePoint(const point_t pt) const;

    bool isIntersectionLine(const triangle_t triag) const;

    bool isIntersectionLineLine(const triangle_t triag) const;

    bool isIntersection3D(const triangle_t& triag) const;
};

} // Geometric