#include <utility>
#include <cmath>
using namespace std;

const double accurasy = 0.0001;

inline bool isEqual(double first, double second) {
    if (abs(first - second) <= accurasy)
        return 1;
    return 0;
}

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

    bool isParallel(const vector_t& vec) const {
        double kX = coord.x / vec.coord.x;
        double kY = coord.y / vec.coord.y;
        double kZ = coord.z / vec.coord.z;

        if ((abs(kX - kY) <= accurasy) && (abs(kX - kZ) <= accurasy))
            return 1;
        return 0;
    }

    vector_t normal() const {
        coord_t thisCoord = this->getCoord();
        vector_t res(thisCoord.y, -thisCoord.x, 0);
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

    vector_t operator*(const double mul) const {
        double x = coord.x * mul;
        double y = coord.y * mul;
        double z = coord.z * mul;

        vector_t res(x, y, z);
        return res;
    }

    vector_t operator+(vector_t vec) {
        double x = coord.x + vec.coord.x;
        double y = coord.y + vec.coord.y;
        double z = coord.z + vec.coord.z;

        vector_t res(x, y, z);
        return res;
    }

    vector_t operator-(vector_t vec) {
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
        coord_t aCoord = lineData.a.getCoord();
        vector_t res(aCoord.y, -aCoord.x, aCoord.z);
        return res;
    }

    double getT(const point_t& pt) const {
        vector_t ptVec = pt.toVector();
        vector_t linePtVec = lineData.linePt.toVector();

        vector_t tmp = ptVec - linePtVec;

        coord_t tmpCoord = tmp.getCoord();
        coord_t dirCoord = lineData.a.getCoord();

        double t = tmpCoord.x / dirCoord.x;
        return t;
    }

    point_t projOfPoint(const point_t& pt) const {
        vector_t ptVec = pt.toVector();
        vector_t linePtVec = lineData.linePt.toVector();

        double tmpCoeff = lineData.a.scalarMult(linePtVec - ptVec) / lineData.a.scalarMult(lineData.a);
        vector_t resVec = linePtVec - lineData.a * tmpCoeff;
        
        coord_t resCoord = resVec.getCoord();
        point_t resPt(resCoord.x, resCoord.y, resCoord.z);

        return resPt;
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

        vector_t norm = (secondVec - firstVec).vectorMult(therdVec - firstVec);

        coord_t normCoord = norm.getCoord();
        double normalCoeff = sqrt(normCoord.x * normCoord.x + normCoord.y * normCoord.y + normCoord.z * normCoord.z);

        norm * (1/normalCoeff);

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

    point_t projOfPoint(const point_t& pt) const {
        plateData_t pltData = this->getData();
        vector_t thisVec = pt.toVector();

        vector_t projVec = thisVec - pltData.n * (thisVec.scalarMult(pltData.n) + pltData.d);
        coord_t vecCoord = projVec.getCoord();

        point_t projPt(vecCoord.x, vecCoord.y, vecCoord.z);
        return projPt;
    }

    vector_t projOfVector( vector_t& vec) const {
        vector_t projVec = vec - pltData.n * (vec.scalarMult(pltData.n));
        return projVec;
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

        vector_t commVec = pltData.n * a + plt.pltData.n * b;
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

    bool checkSepAxe(const line_t& edge, const triangle_t& triang, const plate_t& plt) const {
        vector_t normFirst = edge.normal();
        vector_t projNorm  = plt.projOfVector(normFirst);

        //TODO: check projNorm

        line_t normal(projNorm, trianglePt.first);

        point_t projVert  = normal.projOfPoint(trianglePt.second);
        double vertT = normal.getT(projVert);

        point_t projVert1 = normal.projOfPoint(triang.trianglePt.first);
        double vert1T = normal.getT(projVert1);
        // same sign
        if (vertT * vert1T > 0)
            return 1;
        
        point_t projVert2 = normal.projOfPoint(triang.trianglePt.second);
        double vert2T = normal.getT(projVert2);
        // same sign
        if (vertT * vert2T > 0)
            return 1;

        point_t projVert3 = normal.projOfPoint(triang.trianglePt.third);
        double vert3T = normal.getT(projVert2);
        // same sign
        if (vertT * vert3T > 0)
            return 1;

        // all different sign
        return 0;
    }



    public:
    triangle_t(point_t pt1, point_t pt2, point_t pt3) : trianglePt{{NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}} {
        trianglePt.first.copy(pt1);
        trianglePt.second.copy(pt2);
        trianglePt.third.copy(pt3);
    }

    triangle_t projOntoPlt(const plate_t& plt) {
        point_t projFirst  = plt.projOfPoint(trianglePt.first);
        point_t projSecond = plt.projOfPoint(trianglePt.second);
        point_t projThird  = plt.projOfPoint(trianglePt.third);

        triangle_t triangle(projFirst, projSecond, projThird);
        return triangle;
    }

    bool isIntersection2D(const triangle_t& triang, const plate_t& plt) const {
        line_t firstEdge (trianglePt.first, trianglePt.second);
        line_t secondEdge(trianglePt.second, trianglePt.third);
        line_t thirdEdge (trianglePt.third, trianglePt.first);

        bool chk = checkSepAxe(firstEdge, triang, plt);
        // found intersection
        if(chk)
            return 1;

        chk = checkSepAxe(secondEdge, triang, plt);
        // found intersection
        if(chk)
            return 1;

        chk = checkSepAxe(thirdEdge, triang, plt);
        // found intersection
        if(chk)
            return 1;

        // not found intersection
        return 0;
    }
};

