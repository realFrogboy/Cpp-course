#include <cmath>
using namespace std;

const double accurasy = 0.01;

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
        if ((abs(coord.x) <= accurasy) && (abs(coord.y) <= accurasy) && (abs(coord.z) <= accurasy))
            return 0;
        return 1;
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

    vector_t normal1() const {
        vector_t res(coord.y, -coord.x, 0);
        return res;
    }

    vector_t normal2() const {
        vector_t res(coord.z, 0, -coord.x);
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
        if ((abs(coord.x) <= accurasy) && (abs(coord.y) <= accurasy) && (abs(coord.z) <= accurasy))
            return 0;
        return 1;
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

    bool onLine(const point_t& pt) const {
        if (pt.isEqual(lineData.linePt))
            return 1;

        vector_t ptVec = pt.toVector();
        vector_t linePtVec = lineData.linePt.toVector();
        if (lineData.a.isParallel(linePtVec - ptVec))
            return 1;
        return 0;
    }

    bool isCross(const line_t& line) const {
        // TODO:
    }

    double getT(const point_t& pt) const {
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

    point_t projOfPoint(const point_t& pt) const {
        vector_t ptVec = pt.toVector();
        vector_t linePtVec = lineData.linePt.toVector();

        double tmpCoeff = lineData.a.scalarMult(ptVec - linePtVec) / lineData.a.scalarMult(lineData.a);
        vector_t resVec = linePtVec + lineData.a * tmpCoeff;
        
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

        vector_t normNorm = norm * (1/normalCoeff);

        pltData.n.copy(normNorm);
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

        double r = pltData.n.scalarMult(ptVec) + pltData.d;

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

    bool checkSign(const double& vertT, const line_t& normal, const point_t& vert) const {
        point_t projVert = normal.projOfPoint(vert);
        double vert1T = normal.getT(projVert);
        
        if ((vertT * vert1T) >= 0)
            return 0;
        return 1;
    }

    bool checkSepAxe(const line_t& edge, const triangle_t& triang, const plate_t& plt) const {
        lineData_t edgeData = edge.getData();
        vector_t edgeNorm = edgeData.a.normal1();
        vector_t projNorm  = plt.projOfVector(edgeNorm);
        
        if (!projNorm.valid()) {
            edgeNorm = edgeData.a.normal2();
            projNorm  = plt.projOfVector(edgeNorm);
        }

        //TODO: check projNorm

        line_t normal(projNorm, edgeData.linePt);

        // getting a "good" point to find it's projection on normal with non-null T parametr
        point_t projVert = normal.projOfPoint(trianglePt.second);
        if (edgeData.linePt.isEqual(trianglePt.first)) {
            if (!projVert.valid()) 
                projVert = normal.projOfPoint(trianglePt.third);
        }
        else if (edgeData.linePt.isEqual(trianglePt.second)) {
            projVert = normal.projOfPoint(trianglePt.first);
            if (!projVert.valid()) 
                projVert = normal.projOfPoint(trianglePt.third);
        }
        else {
            projVert = normal.projOfPoint(trianglePt.first);
            if (!projVert.valid())
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

    int getDist(const double dist1, const double dist2, const double dist3, 
                                const point_t pt1, const point_t pt2, const point_t pt3) const 
    {
        if ((dist2 * dist1 <= 0) && (dist3 * dist1 <= 0)) {
            if (abs(dist1) < accurasy) {
                return 23;
            else 
                return 1;
        }

        if ((dist1 * dist2 <= 0) && (dist3 * dist2 <= 0)) {
            if (abs(dist2) < accurasy) {
                return 13;
            else 
                return 2;
        }

        if ((dist1 * dist3 <= 0) && (dist2 * dist3 <= 0)) {
            if (abs(dist3) < accurasy) {
                return 12;
            else
                return 3;
        }
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

    bool isIntersection3D(const triangle_t& triag) const {
        //TODO: situations

        plate_t plt1(trianglePt.first, trianglePt.second, trianglePt.third);

        double dist21 = plt1.distToPoint(triag.trianglePt.first);
        double dist22 = plt1.distToPoint(triag.trianglePt.second);
        double dist23 = plt1.distToPoint(triag.trianglePt.third);

        if ((dist21 * dist22 > 0) && (dist21 * dist23 > 0))
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

        if ((dist11 * dist12 > 0) && (dist11 * dist13 > 0))
            return 0;

        line_t interLine(plt1, plt2);
        lineData_t interData = interLine.getData();
        vector_t linePtVec = interData.linePt.toVector();

        auto p1 = getDist(dist11, dist12, dist13, trianglePt.first, trianglePt.second, trianglePt.third);
        auto p2 = getDist(dist21, dist22, dist23, triag.trianglePt.first, triag.trianglePt.second, triag.trianglePt.third);

        vector_t vec11 = trianglePt.first.toVector();
        vector_t vec12 = trianglePt.second.toVector();
        vector_t vec13 = trianglePt.third.toVector();

        vector_t vec21 = triag.trianglePt.first.toVector();
        vector_t vec22 = triag.trianglePt.second.toVector();
        vector_t vec23 = triag.trianglePt.third.toVector();

        double projVec11 = interData.a.scalarMult(vec11 - linePtVec);
        double projVec12 = interData.a.scalarMult(vec12 - linePtVec);
        double projVec13 = interData.a.scalarMult(vec13 - linePtVec);

        double projVec21 = interData.a.scalarMult(vec21 - linePtVec);
        double projVec22 = interData.a.scalarMult(vec22 - linePtVec);
        double projVec23 = interData.a.scalarMult(vec23 - linePtVec);

        double t11 = 0, t12 = 0, t21 = 0, t22 = 0;

        switch(p1) {
            case 1:
                t11 = projVec12 + (projVec11 - projVec12) * dist12 / (dist12 - dist11);
                t12 = projVec13 + (projVec11 - projVec13) * dist13 / (dist13 - dist11);
                break;
            
            case 2:
                t11 = projVec11 + (projVec12 - projVec11) * dist11 / (dist11 - dist12);
                t12 = projVec13 + (projVec12 - projVec13) * dist13 / (dist13 - dist12);
                break;

            case 3:
                t11 = projVec11 + (projVec13 - projVec11) * dist11 / (dist11 - dist13);
                t12 = projVec12 + (projVec13 - projVec12) * dist12 / (dist12 - dist13);
                break;

            case 23:
                t11 = interLine.getT(trianglePt.first);
                t12 = t11;
                break;

            case 13:
                t11 = interLine.getT(trianglePt.second);
                t12 = t11;
                break;
            
            case 12:
                t11 = interLine.getT(trianglePt.third);
                t12 = t11;     
        }

        switch(p2) {
            case 1:
                t21 = projVec22 + (projVec21 - projVec22) * dist22 / (dist22 - dist21);
                t22 = projVec23 + (projVec21 - projVec23) * dist23 / (dist23 - dist21);
                break;
            
            case 2:
                t21 = projVec21 + (projVec22 - projVec21) * dist21 / (dist21 - dist22);
                t22 = projVec23 + (projVec22 - projVec23) * dist23 / (dist23 - dist22);
                break;

            case 3:
                t21 = projVec21 + (projVec23 - projVec21) * dist21 / (dist21 - dist23);
                t22 = projVec22 + (projVec23 - projVec22) * dist22 / (dist22 - dist23);
                break;

            case 23:
                t21 = interLine.getT(triag.trianglePt.first);
                t22 = t21;
                break;

            case 13:
                t21 = interLine.getT(triag.trianglePt.second);
                t22 = t21;
                break;
            
            case 12:
                t21 = interLine.getT(triag.trianglePt.third);
                t22 = t21;     
        }
        
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
};

