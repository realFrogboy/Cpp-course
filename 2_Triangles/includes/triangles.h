#include <utility>
#include <cmath>
using namespace std;

const double accurasy = 0.0001;

struct coord_t {
    double x;
    double y;
    double z;
};


class vector_t {
    coord_t coord{NAN, NAN, NAN};

    public:
    vector_t(double x, double y, double z) : coord{x, y, z} {};

    coord_t getCoord() const { return coord; };

    void copy(const vector_t& vec) {
        coord.x = vec.coord.x;
        coord.y = vec.coord.y;
        coord.z = vec.coord.z;
    }

    bool valid() const {
        if ((isfinite(coord.x)) && (isfinite(coord.y)) && (isfinite(coord.z)))
            return 1;
        return 0;
    }

    bool isEqual(const vector_t& vec) const {
        if ((abs(coord.x - vec.coord.x) <= accurasy) && (abs(coord.y - vec.coord.y) <= accurasy) && (abs(coord.z - vec.coord.z) <= accurasy))
            return 1;
        return 0;
    }

    vector_t constMult(const double mul) const {
        double x = coord.x * mul;
        double y = coord.y * mul;
        double z = coord.z * mul;

        vector_t res(x, y, z);
        return res;
    }

    double scalarMult(const vector_t& vec) const { 
        return (coord.x * vec.coord.x + coord.y * vec.coord.y + coord.z * vec.coord.z); 
    }

    vector_t vectorMult(const vector_t& vec) const {
        double x = coord.y * vec.coord.z - coord.z * vec.coord.y;
        double y = coord.z * vec.coord.x - coord.x * vec.coord.z;
        double z = coord.x * vec.coord.y - coord.y * vec.coord.x;

        vector_t res(x, y, z);
        return res;
    }

    vector_t operator+(vector_t& vec) {
        double x = coord.x + vec.coord.x;
        double y = coord.y + vec.coord.y;
        double z = coord.z + vec.coord.z;

        vector_t res(x, y, z);
        return res;
    }

    vector_t operator-(vector_t& vec) {
        double x = coord.x - vec.coord.x;
        double y = coord.y - vec.coord.y;
        double z = coord.z - vec.coord.z;

        vector_t res(x, y, z);
        return res;
    }

    double length() {
       double tmp =  this->scalarMult(*this);
       return sqrt(tmp);
    }
};

class point_t {
    coord_t coord{NAN, NAN, NAN};

    public:
    point_t(double x, double y, double z) : coord{x, y, z} {};

    coord_t getCoord() const { return coord; }

    void copy(const point_t& pt) {
        coord.x = pt.coord.x;
        coord.y = pt.coord.y;
        coord.z = pt.coord.z;
    }
    
    bool valid() const {
        if ((isfinite(coord.x)) && (isfinite(coord.y)) && (isfinite(coord.z)))
            return 1;
        return 0;
    }

    bool isEqual(const point_t& pt) const {
        if ((abs(coord.x - pt.coord.x) <= accurasy) && (abs(coord.y - pt.coord.y) <= accurasy) && (abs(coord.z - pt.coord.z) <= accurasy))
            return 1;
        return 0;
    }

    vector_t toVector() const {
        vector_t vec(coord.x, coord.y, coord.z);
        return vec;
    }
};


struct lineData_t {
    vector_t a;
    point_t linePt;
};

class line_t {
    lineData_t lineData{{NAN, NAN, NAN}, {NAN, NAN, NAN}};

    public:
    line_t(vector_t dir, point_t pt) : lineData{{NAN, NAN, NAN}, {NAN, NAN, NAN}} {
        lineData.a.copy(dir);
        lineData.linePt.copy(pt);
    }

    line_t(const point_t& firstPt, const point_t& secondPt) : lineData{{NAN, NAN, NAN}, {NAN, NAN, NAN}} {
        vector_t firstVec  = firstPt.toVector();
        vector_t secondVec = secondPt.toVector();

        lineData.a.copy(secondVec - firstVec);
        lineData.linePt.copy(firstPt);
    }

    lineData_t getData() const { return lineData; }

    bool valid() const {
        if ((lineData.a.valid()) && (lineData.linePt.valid()))
            return 1;
        return 0;
    }

    vector_t normal() const {

    }
};


struct plateData_t {
    vector_t n;
    point_t platePt;
    double d;
};

class plate_t {
    plateData_t pltData {{NAN, NAN, NAN}, {NAN, NAN, NAN}, NAN};

    public:
    plate_t(const point_t& firstPt, const point_t& secondPt, const point_t& therdPt) : pltData {{NAN, NAN, NAN}, {NAN, NAN, NAN}, NAN} {
        vector_t firstVec  = firstPt.toVector();
        vector_t secondVec = secondPt.toVector();
        vector_t therdVec  = therdPt.toVector();

        vector_t tmp = (secondVec - firstVec).vectorMult(therdVec - firstVec);
        coord_t tmpCoord = tmp.getCoord();
        double normalCoeff = sqrt(tmpCoord.x * tmpCoord.x + tmpCoord.y * tmpCoord.y + tmpCoord.z * tmpCoord.z);
        tmpCoord.x /= normalCoeff;
        tmpCoord.y /= normalCoeff;
        tmpCoord.z /= normalCoeff;

        vector_t norm(tmpCoord.x, tmpCoord.y, tmpCoord.z);

        pltData.n.copy(norm);
        pltData.platePt.copy(firstPt);
        pltData.d = -firstVec.scalarMult(pltData.n);
    }

    plateData_t getData() const { return pltData; }

    bool valid() {
        if ((pltData.n.valid()) && (pltData.platePt.valid()) && (isfinite(pltData.d)))
            return 1;
        return 0;
    }

    bool isParallel(const plate_t& plt) const {
        if ((pltData.n.isEqual(plt.pltData.n)) && (pltData.platePt.isEqual(plt.pltData.platePt)))
            return 1;
        return 0;
    }

    bool isEqual(const plate_t& plt) const {
        if ((pltData.n.isEqual(plt.pltData.n)) && (pltData.platePt.isEqual(plt.pltData.platePt)) && (abs(pltData.d - plt.pltData.d) <= accurasy))
            return 1;
        return 0;
    }

    double distToPoint(const point_t& pt) const {
        vector_t ptVec = pt.toVector();
        vector_t plateVec = pltData.platePt.toVector();

        vector_t vec = ptVec - plateVec;

        double dist = abs(pltData.n.scalarMult(vec));
        double r = pltData.n.scalarMult(ptVec) + dist;

        return r;
    }

    point_t projOfPt(const point_t& pt) const {
        plateData_t pltData = this->getData();
        vector_t thisVec = pt.toVector();

        vector_t projVec = thisVec - pltData.n.constMult((thisVec.scalarMult(pltData.n) + pltData.d));
        coord_t vecCoord = projVec.getCoord();

        point_t pt(vecCoord.x, vecCoord.y, vecCoord.z);
        return pt;
    }

    line_t intsectOf2Plt(const plate_t& plt) const {
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

        vector_t commVec = pltData.n.constMult(a) + plt.pltData.n.constMult(b);
        coord_t coords = commVec.getCoord();
        point_t commPt(coords.x, coords.y, coords.z);

        line_t line(aVec, commPt);
        return line;
    }
};


struct trianglePt_t {
    point_t first;
    point_t second;
    point_t third;
};

class triangle_t {
    trianglePt_t trianglePt{{NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}};

    public:
    triangle_t(point_t pt1, point_t pt2, point_t pt3) : trianglePt{{NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}} {
        trianglePt.first.copy(pt1);
        trianglePt.second.copy(pt2);
        trianglePt.third.copy(pt3);
    }

    triangle_t projOntoPlt(const plate_t& plt) {
        point_t projFirst  = plt.projOfPt(trianglePt.first);
        point_t projSecond = plt.projOfPt(trianglePt.second);
        point_t projThird  = plt.projOfPt(trianglePt.third);

        triangle_t triangle(projFirst, projSecond, projThird);
        return triangle;
    }

    bool intersection2D(const triangle_t& triang) const {

    }
};