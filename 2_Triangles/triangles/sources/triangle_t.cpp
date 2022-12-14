#include "triangle_t.h"

#include <limits>
#include <cassert>

namespace Triangles {
namespace Geometric {

triangle_t triangle_t::projOntoPlt(const plate_t& plt) const {
    point_t projFirst  = plt.projOfPoint(trianglePt.first);
    point_t projSecond = plt.projOfPoint(trianglePt.second);
    point_t projThird  = plt.projOfPoint(trianglePt.third);

    triangle_t triangle(projFirst, projSecond, projThird);
    return triangle;
}

std::pair<point_t, point_t> triangle_t::getMinMax() const {
    line_t line(trianglePt.first, trianglePt.second);
    double t2 = line.getT(trianglePt.second);
    double t3 = line.getT(trianglePt.third);

    if (t2 * t3 <= 0)
        return std::make_pair(trianglePt.second, trianglePt.third);
    else if ((t2 > 0) && (t2 <= t3))
        return std::make_pair(trianglePt.first, trianglePt.third);
    else if ((t2 > 0) && (t3 <= t2))
        return std::make_pair(trianglePt.first, trianglePt.second);
    else if ((t2 < 0) && (t2 >= t3))
        return std::make_pair(trianglePt.first, trianglePt.third);
    else 
        return std::make_pair(trianglePt.first, trianglePt.second);
}

bool triangle_t::isIntersectionWithPoint(const triangle_t triag) const {
    plate_t plt(trianglePt.first, trianglePt.second, trianglePt.third);
    if (std::abs(plt.distToPoint(triag.trianglePt.first)) > accuracy)
        return 0;

    return isIntersection2D(triag, plt);
}

bool triangle_t::isIntersectionLinePoint(const point_t pt) const {
    auto pr = getMinMax();
    line_t lineSegment(pr.first, pr.second);

    if (!lineSegment.onLine(pt))
        return 0;

    double t = lineSegment.getT(pt);
    if ((t <= 1) && (t >= 0))
        return 1;
    else 
        return 0;
}

bool triangle_t::isIntersectionLine(const triangle_t triag) const {
    plate_t plt(trianglePt.first, trianglePt.second, trianglePt.third);
    plateData_t pltData = plt.pltData;

    auto pr = triag.getMinMax();
    line_t line(pr.first, pr.second);
    lineData_t lineData = line.lineData;

    if (std::abs(pltData.n.scalarMult(lineData.a)) < accuracy) {
        if (std::abs(plt.distToPoint(lineData.linePt)) < accuracy)
            return isIntersection2D(triag, plt);
        else
            return 0;
    }
        
    point_t inter = plt.intersectionLine(line);
    double t = line.getT(inter);
    if ((t > 1) || (t < 0)) 
        return 0;

    triangle_t tmp(inter, inter, inter); 
    return isIntersectionWithPoint(tmp);
}

bool triangle_t::isIntersectionLineLine(const triangle_t triag) const {
    auto pr1 = getMinMax();
    auto pr2 = triag.getMinMax();

    line_t line1(pr1.first, pr1.second);
    line_t line2(pr2.first, pr2.second);
    point_t pt = line1.shortestLine(line2);

    if (!pt.isValid())
        return 0;

    double t1 = line1.getT(pt);
    double t2 = line2.getT(pt);
    if ((t1 <= 1) && (t1 >= 0) && (t2 <= 1) && (t2 >= 0))
        return 1;
    return 0;
}

int triangle_t::checkSpecialCases(const triangle_t& triag) const {
    bool chkPt1 = isPoint();
    bool chkPt2 = triag.isPoint();

    bool chkLine1 = isLine();
    bool chkLine2 = triag.isLine();

    if (chkPt1) {
        if (chkPt2)
            return trianglePt.first.isEqual(triag.trianglePt.first);
        else if (chkLine2)
            return triag.isIntersectionLinePoint(trianglePt.first);
        else 
             return triag.isIntersectionWithPoint(*this);
    }

    if (chkPt2) {
        if (chkLine1)
            return isIntersectionLinePoint(triag.trianglePt.first);
        else 
             return isIntersectionWithPoint(triag);
    }

    if (chkLine1) {
        if (chkLine2) 
            return isIntersectionLineLine(triag);
         else 
            return triag.isIntersectionLine(*this);
    }

    if(chkLine2)
        return isIntersectionLine(triag);
    
    return 2;
}

bool triangle_t::checkSepAxe(const line_t& edge, const triangle_t& triang, const plate_t& plt) const {
    lineData_t edgeData = edge.lineData;
    vector_t edgeNorm = edgeData.a.normal1();
    vector_t projNorm  = plt.projOfVector(edgeNorm);
    
    if (!projNorm.isNull()) {
        edgeNorm = edgeData.a.normal2();
        projNorm  = plt.projOfVector(edgeNorm);
    }

    line_t normal(projNorm, edgeData.linePt);

    point_t projVert = normal.projOfPoint(trianglePt.second);

    if (edgeData.linePt.isEqual(projVert))
        projVert = normal.projOfPoint(trianglePt.third);

    if (edgeData.linePt.isEqual(projVert))
        projVert = normal.projOfPoint(trianglePt.first);
    
    double vertT = normal.getT(projVert);

    bool chk = checkSign(vertT, normal, triang.trianglePt.first);
    if(chk)
        return 0;

    chk = checkSign(vertT, normal, triang.trianglePt.second);
    if(chk)
        return 0;

    chk = checkSign(vertT, normal, triang.trianglePt.third);
    if(chk)
        return 0;

    // all different sign: division founded
    return 1;
}

int triangle_t::getDist(const double dist1, const double dist2, const double dist3) const {
    if (((dist2 * dist1 < 0) || (std::abs(dist2) < accuracy)) && ((dist3 * dist1 < 0) || (std::abs(dist3) < accuracy)))
            return 1;

    if (((dist1 * dist2 < 0) || (std::abs(dist1) < accuracy)) && ((dist3 * dist2 < 0) || (std::abs(dist3) < accuracy)))        
            return 2;

    if (((dist1 * dist3 < 0) || (std::abs(dist1) < accuracy)) && ((dist2 * dist3 < 0) || (std::abs(dist2) < accuracy)))
            return 3;

    if (std::abs(dist1) < accuracy) 
        return 23;

    if (std::abs(dist2) < accuracy) 
        return 13;

    if (std::abs(dist3) < accuracy)
        return 12;
    
    return 0;
}

std::pair<double, double> triangle_t::getTT(const triangle_t& triag, const line_t& interLine, const int p) const {
    double t11 = 0, t12 = 0;
    switch(p) {
        case 1: {
            line_t line1(triag.trianglePt.first, triag.trianglePt.second);
            line_t line2(triag.trianglePt.first, triag.trianglePt.third);

            point_t pt1 = interLine.shortestLine(line1);
            assert(pt1.isValid());
            
            point_t pt2 = interLine.shortestLine(line2);
            assert(pt1.isValid());

            t11 = interLine.getT(pt1);
            t12 = interLine.getT(pt2);
            break;
        }
        
        case 2: {
            line_t line1(triag.trianglePt.second, triag.trianglePt.first);
            line_t line2(triag.trianglePt.second, triag.trianglePt.third);

            point_t pt1 = interLine.shortestLine(line1);
            assert(pt1.isValid());
            
            point_t pt2 = interLine.shortestLine(line2);
            assert(pt1.isValid());

            t11 = interLine.getT(pt1);
            t12 = interLine.getT(pt2);
            break;
        }

        case 3: {
            line_t line1(triag.trianglePt.third, triag.trianglePt.first);
            line_t line2(triag.trianglePt.third, triag.trianglePt.second);

            point_t pt1 = interLine.shortestLine(line1);
            assert(pt1.isValid());
            
            point_t pt2 = interLine.shortestLine(line2);
            assert(pt1.isValid());

            t11 = interLine.getT(pt1);
            t12 = interLine.getT(pt2);
            break;
        }

        case 23: {
            t11 = interLine.getT(triag.trianglePt.first);
            t12 = t11;
            break;
        }

        case 13: {
            t11 = interLine.getT(triag.trianglePt.second);
            t12 = t11;
            break;
        }
        
        case 12: {
            t11 = interLine.getT(triag.trianglePt.third);
            t12 = t11;
        }     
    }

    std::pair<double, double> pr = std::make_pair(t11, t12);
    return pr;
}

bool triangle_t::isIntersection2D(const triangle_t& triang, const plate_t& plt) const {
    line_t firstEdge (trianglePt.first, trianglePt.second);
    line_t secondEdge(trianglePt.second, trianglePt.third);
    line_t thirdEdge (trianglePt.third, trianglePt.first);

    bool chk = checkSepAxe(firstEdge, triang, plt);
    if(chk)
        return 0;

    chk = checkSepAxe(secondEdge, triang, plt);
    if(chk)
        return 0;

    chk = checkSepAxe(thirdEdge, triang, plt);
    if(chk)
        return 0;

    // found intersection
    return 1;
}

bool triangle_t::isIntersection3D(const triangle_t& triag) const {
    int chk = checkSpecialCases(triag);
    if(chk == 1)
        return 1;
    else if (chk == 0)
        return 0;

    plate_t plt1(trianglePt.first, trianglePt.second, trianglePt.third);

    double dist21 = plt1.distToPoint(triag.trianglePt.first);
    double dist22 = plt1.distToPoint(triag.trianglePt.second);
    double dist23 = plt1.distToPoint(triag.trianglePt.third);

    if ((dist21 * dist22 > 0) && (dist21 * dist23 > 0) && (std::abs(dist21) > accuracy) && (std::abs(dist22) > accuracy) && (std::abs(dist23) > accuracy))
        return 0;

    plate_t plt2(triag.trianglePt.first, triag.trianglePt.second, triag.trianglePt.third);

    if (plt1.isEqual(plt2)) {
        triangle_t projTriag = triag.projOntoPlt(plt1);
        bool res = isIntersection2D(projTriag, plt1);
        return res;
    } else if (plt1.isParallel(plt2)) 
        return 0;

    double dist11 = plt2.distToPoint(trianglePt.first);
    double dist12 = plt2.distToPoint(trianglePt.second);
    double dist13 = plt2.distToPoint(trianglePt.third);

    if ((dist11 * dist12 > 0) && (dist11 * dist13 > 0) && (std::abs(dist11) > accuracy) && (std::abs(dist12) > accuracy) && (std::abs(dist13) > accuracy))
        return 0;

    line_t interLine = plt1.intsectOf2Plt(plt2);

    auto p1 = getDist(dist11, dist12, dist13);
    auto p2 = getDist(dist21, dist22, dist23);

    std::pair<double, double> pr1 = getTT(*this, interLine, p1);
    std::pair<double, double> pr2 = getTT(triag, interLine, p2);

    double t11 = pr1.first, t12 = pr1.second, t21 = pr2.first, t22 = pr2.second;

    if ((std::abs(t11 - t21) < accuracy) || (std::abs(t11 - t22) < accuracy) || (std::abs(t12 - t21) < accuracy) || (std::abs(t12 - t22) < accuracy))
        return 1;
    
    if (((t11 <= t12) && (t12 < t21) && (t21 <= t22)) ||
        ((t11 <= t12) && (t12 < t22) && (t22 <= t21)) ||
        ((t12 <= t11) && (t11 < t21) && (t21 <= t22)) ||
        ((t12 <= t11) && (t11 < t22) && (t22 <= t21)) ||

        ((t21 <= t22) && (t22 < t11) && (t11 <= t12)) ||
        ((t21 <= t22) && (t22 < t12) && (t12 <= t11)) ||
        ((t22 <= t21) && (t21 < t11) && (t11 <= t12)) ||
        ((t22 <= t21) && (t21 < t12) && (t12 <= t11))) {
        return 0;
    }
    return 1;
}

point_t triangle_t::getMin() const {
    Triangles::Geometric::trianglePt_t triagPt = trianglePt;

    coord_t coordPt1 = triagPt.first.coord;
    coord_t coordPt2 = triagPt.second.coord;
    coord_t coordPt3 = triagPt.third.coord;

    double minX = 0;
    (coordPt1.x < coordPt2.x) ? minX = coordPt1.x : minX = coordPt2.x;
    if (minX > coordPt3.x)
        minX = coordPt3.x;

    double minY = 0;
    (coordPt1.y < coordPt2.y) ? minY = coordPt1.y : minY = coordPt2.y;
    if (minY > coordPt3.y)
        minY = coordPt3.y;

    double minZ = 0;
    (coordPt1.z < coordPt2.z) ? minZ = coordPt1.z : minZ = coordPt2.z;
    if (minZ > coordPt3.z)
        minZ = coordPt3.z;

    point_t res {minX, minY, minZ};
    return res;
}

point_t triangle_t::getMax() const {
    Triangles::Geometric::trianglePt_t triagPt = trianglePt;

    coord_t coordPt1 = triagPt.first.coord;
    coord_t coordPt2 = triagPt.second.coord;
    coord_t coordPt3 = triagPt.third.coord;

    double maxX = 0;
    (coordPt1.x > coordPt2.x) ? maxX = coordPt1.x : maxX = coordPt2.x;
    if (maxX < coordPt3.x)
        maxX = coordPt3.x;

    double maxY = 0;
    (coordPt1.y > coordPt2.y) ? maxY = coordPt1.y : maxY = coordPt2.y;
    if (maxY < coordPt3.y)
        maxY = coordPt3.y;

    double maxZ = 0;
    (coordPt1.z > coordPt2.z) ? maxZ = coordPt1.z : maxZ = coordPt2.z;
    if (maxZ < coordPt3.z)
        maxZ = coordPt3.z;

    point_t res {maxX, maxY, maxZ};
    return res;
}

} // Geometric
} // Triangles