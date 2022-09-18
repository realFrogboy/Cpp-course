#include "plate_t.h"

plate_t::plate_t(const point_t& firstPt, const point_t& secondPt, const point_t& therdPt) : pltData {{NAN, NAN, NAN}, {NAN, NAN, NAN}, NAN} {
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

line_t plate_t::intsectOf2Plt(const plate_t& plt) const {
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