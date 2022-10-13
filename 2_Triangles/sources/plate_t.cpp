#include "plate_t.h"

namespace Geometric {

bool plate_t::valid() const {
    if ((pltData.n.isValid()) && (pltData.platePt.isValid()) && (std::isfinite(pltData.d)))
        return 1;
    return 0;
}

bool plate_t::isParallel(const plate_t& plt) const {
    if (pltData.n.isParallel(plt.pltData.n))
        return 1;
    return 0;
}

bool plate_t::isEqual(const plate_t& plt) const {
    if ((pltData.n.isParallel(plt.pltData.n)) && (std::abs(pltData.d - plt.pltData.d) <= accuracy))
        return 1;
    return 0;
}

double plate_t::distToPoint(const point_t& pt) const {
    vector_t ptVec = pt.toVector();
    double r = pltData.n.scalarMult(ptVec) + pltData.d;
    return r;
}

point_t plate_t::projOfPoint(const point_t& pt) const {
    vector_t thisVec = pt.toVector();

    vector_t projVec = thisVec - pltData.n * (thisVec.scalarMult(pltData.n) + pltData.d);
    coord_t vecCoord = projVec.coord;

    point_t projPt(vecCoord.x, vecCoord.y, vecCoord.z);
    return projPt;
}

vector_t plate_t::projOfVector( vector_t& vec) const {
    vector_t projVec = vec - pltData.n * (vec.scalarMult(pltData.n));
    return projVec;
}

plate_t::plate_t(const point_t& firstPt, const point_t& secondPt, const point_t& therdPt) {
    vector_t firstVec  = firstPt.toVector();
    vector_t secondVec = secondPt.toVector();
    vector_t therdVec  = therdPt.toVector();

    vector_t norm = (secondVec - firstVec).vectorMult(therdVec - firstVec);

    coord_t normCoord = norm.coord;
    double normalCoeff = sqrt(normCoord.x * normCoord.x + normCoord.y * normCoord.y + normCoord.z * normCoord.z);

    vector_t normNorm = norm * (1/normalCoeff);

    pltData.n.copy(normNorm);
    pltData.platePt.copy(firstPt);
    pltData.d = -firstVec.scalarMult(pltData.n);
}

line_t plate_t::intsectOf2Plt(const plate_t& plt) const {
    // parallel or equal plates
    if(isParallel(plt)) {
        line_t res{};
        return res;
    }

    // not parallel or equal plates
    vector_t aVec = pltData.n.vectorMult(plt.pltData.n);
    double s1 = -pltData.d;
    double s2 = -plt.pltData.d;

    double a = getCoeff(plt, s1, s2);
    double b = getCoeff(plt, s2, s1);

    vector_t commVec = pltData.n * a + plt.pltData.n * b;
    coord_t coords = commVec.coord;
    point_t commPt(coords.x, coords.y, coords.z);

    line_t line(aVec, commPt);
    return line;
}

point_t plate_t::intersectionLine(const line_t& line) const {
    lineData_t lineData = line.lineData;
    vector_t linePtVec = lineData.linePt.toVector();

    double t = -(pltData.n.scalarMult(linePtVec) + pltData.d) / pltData.n.scalarMult(lineData.a);

    vector_t resVec = linePtVec + lineData.a * t;
    coord_t resCoord = resVec.coord;
    
    point_t pt(resCoord.x, resCoord.y, resCoord.z);
    return pt;
}

double plate_t::getCoeff(const plate_t& plt, const double s1, const double s2) const {
    return  (s2 * pltData.n.scalarMult(plt.pltData.n) - s1 * plt.pltData.n.scalarMult(plt.pltData.n)) / 
            (pltData.n.scalarMult(plt.pltData.n) * pltData.n.scalarMult(plt.pltData.n) - 
                pltData.n.scalarMult(pltData.n) * plt.pltData.n.scalarMult(plt.pltData.n));
}

} // Geomrtric