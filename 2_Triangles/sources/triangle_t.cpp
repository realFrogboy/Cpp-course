#include "triangle_t.h"

int triangle_t::checkSpecialCases(const triangle_t& triag) const {
    bool chkPt1 = this->isPoint();
    bool chkPt2 = triag.isPoint();

    if ((chkPt1) && (chkPt2)) 
        if (trianglePt.first.isEqual(triag.trianglePt.first))
            return 1;

    bool chkLine1 = this->isLine();
    bool chkLine2 = triag.isLine();

    if ((chkLine1) && (chkLine2)) {
        line_t line1(trianglePt.first, trianglePt.second);
        line_t line2(triag.trianglePt.first, triag.trianglePt.second);
        point_t pt = line1.isCross(line2);
        if (pt.isValid())
            return 1;
    }

    if ((chkLine1) && (chkPt2)) {
        line_t line(trianglePt.first, trianglePt.second);
        if (line.onLine(trianglePt.first))
            return 1;
    }

    if ((chkPt1) && (chkLine2)) {
        line_t line(triag.trianglePt.first, triag.trianglePt.second);
        if (line.onLine(triag.trianglePt.first))
            return 1;
    }

    return 0;
}

bool triangle_t::checkSepAxe(const line_t& edge, const triangle_t& triang, const plate_t& plt) const {
    lineData_t edgeData = edge.getData();
    vector_t edgeNorm = edgeData.a.normal1();
    vector_t projNorm  = plt.projOfVector(edgeNorm);
    
    if (!projNorm.isNull()) {
        edgeNorm = edgeData.a.normal2();
        projNorm  = plt.projOfVector(edgeNorm);
    }

    line_t normal(projNorm, edgeData.linePt);

    point_t projVert = normal.projOfPoint(trianglePt.second);
    if (edgeData.linePt.isEqual(trianglePt.first)) {
        if (!projVert.isNull()) 
            projVert = normal.projOfPoint(trianglePt.third);
    }
    else if (edgeData.linePt.isEqual(trianglePt.second)) {
        projVert = normal.projOfPoint(trianglePt.first);
        if (!projVert.isNull()) 
            projVert = normal.projOfPoint(trianglePt.third);
    }
    else {
        projVert = normal.projOfPoint(trianglePt.first);
        if (!projVert.isNull())
            projVert = normal.projOfPoint(trianglePt.second);
    }
    
    double vertT = normal.getT(projVert);

    bool chk = checkSign(vertT, normal, triang.trianglePt.first);
    if(!chk)
        return 0;

    chk = checkSign(vertT, normal, triang.trianglePt.second);
    if(!chk)
        return 0;

    chk = checkSign(vertT, normal, triang.trianglePt.third);
    if(!chk)
        return 0;

    // all different sign
    return 1;
}

int triangle_t::getDist(const double dist1, const double dist2, const double dist3) const {
    if (((dist2 * dist1 < 0) && (dist3 * dist1 < 0)) || ((abs(dist2) < accurasy) && (abs(dist3) < accurasy)))
            return 1;

    if (((dist1 * dist2 < 0) && (dist3 * dist2 < 0)) || ((abs(dist1) < accurasy) && (abs(dist3) < accurasy)))
            return 2;

    if (((dist1 * dist3 < 0) && (dist2 * dist3 < 0)) || ((abs(dist1) < accurasy) && (abs(dist2) < accurasy)))
            return 3;

    if (abs(dist1) < accurasy) 
        return 23;

    if (abs(dist2) < accurasy) 
        return 13;

    if (abs(dist3) < accurasy)
        return 12;
    
    return 0;
}

pair<double, double> triangle_t::getTT(const triangle_t& triag, const line_t& interLine, const int p) const {
    double t11 = 0, t12 = 0;
    switch(p) {
        case 1: {
            line_t line1(triag.trianglePt.first, triag.trianglePt.second);
            line_t line2(triag.trianglePt.first, triag.trianglePt.third);

            point_t pt1 = interLine.isCross(line1);
            assert(pt1.isValid());
            
            point_t pt2 = interLine.isCross(line2);
            assert(pt1.isValid());

            t11 = interLine.getT(pt1);
            t12 = interLine.getT(pt2);
            break;
        }
        
        case 2: {
            line_t line1(triag.trianglePt.second, triag.trianglePt.first);
            line_t line2(triag.trianglePt.second, triag.trianglePt.third);

            point_t pt1 = interLine.isCross(line1);
            assert(pt1.isValid());
            
            point_t pt2 = interLine.isCross(line2);
            assert(pt1.isValid());

            t11 = interLine.getT(pt1);
            t12 = interLine.getT(pt2);
            break;
        }

        case 3: {
            line_t line1(triag.trianglePt.third, triag.trianglePt.first);
            line_t line2(triag.trianglePt.third, triag.trianglePt.second);

            point_t pt1 = interLine.isCross(line1);
            assert(pt1.isValid());
            
            point_t pt2 = interLine.isCross(line2);
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

    pair<double, double> pr = make_pair(t11, t12);
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
    if(chk)
        return 1;

    plate_t plt1(trianglePt.first, trianglePt.second, trianglePt.third);

    double dist21 = plt1.distToPoint(triag.trianglePt.first);
    double dist22 = plt1.distToPoint(triag.trianglePt.second);
    double dist23 = plt1.distToPoint(triag.trianglePt.third);

    if ((dist21 * dist22 > 0) && (dist21 * dist23 > 0) && (abs(dist21) > accurasy) && (abs(dist22) > accurasy) && (abs(dist23) > accurasy))
        return 0;

    plate_t plt2(triag.trianglePt.first, triag.trianglePt.second, triag.trianglePt.third);

    if (plt1.isEqual(plt2)) {
        triangle_t projTriag = triag.projOntoPlt(plt1);
        bool res = this->isIntersection2D(projTriag, plt1);
        return res;
    } else if (plt1.isParallel(plt2)) 
        return 0;

    double dist11 = plt2.distToPoint(trianglePt.first);
    double dist12 = plt2.distToPoint(trianglePt.second);
    double dist13 = plt2.distToPoint(trianglePt.third);

    if ((dist11 * dist12 > 0) && (dist11 * dist13 > 0) && (abs(dist11) > accurasy) && (abs(dist12) > accurasy) && (abs(dist13) > accurasy))
        return 0;

    line_t interLine = plt1.intsectOf2Plt(plt2);

    auto p1 = getDist(dist11, dist12, dist13);
    auto p2 = getDist(dist21, dist22, dist23);

    pair<double, double> pr1 = getTT(*this, interLine,p1);
    pair<double, double> pr2 = getTT(triag, interLine,p2);

    double t11 = pr1.first, t12 = pr1.second, t21 = pr2.first, t22 = pr2.second;
    
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
