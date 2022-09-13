#include <utility>
#include <cmath>
using namespace std;

const double accurasy = 0.0001;

struct coord_t {
    double x;
    double y;
    double z;
};

struct coeffs_t {
    double a;
    double b;
    double c;
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

    double scalarMult (const vector_t& vec) const { 
        return (coord.x * vec.coord.x + coord.y * vec.coord.y + coord.z * vec.coord.z); 
    }

    vector_t vectorMult (const vector_t& vec) const {
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

/*class line_t { //TODO: 3d
    coeffs_t coeffs{NAN, NAN, NAN};

    public:
    line_t(double a, double b, double c) : coeffs{a, b, c} {};
    line_t(const point_t& firstPt, const point_t& secondPt) {
        coord_t firstCoord  = firstPt.getCoord();
        coord_t secondCoord = secondPt.getCoord();

        double a = (firstCoord.y - secondCoord.y) / (secondCoord.x - firstCoord.x);
        double c = -a * firstCoord.x - firstCoord.y;

        coeffs.a = a;
        coeffs.b = 1.0;
        coeffs.c = c;
    };

    coeffs_t getCoeffs() const { return coeffs; }

    bool valid() const {
        if ((isfinite(coeffs.a)) && (isfinite(coeffs.b)) && (isfinite(coeffs.c)))
            return 1;
        return 0;
    };

    point_t isCross(const line_t& line) const {
        double kA = coeffs.a / line.coeffs.a;
        double kB = coeffs.b / line.coeffs.b;

        // parallel or equal lines
        if (abs(kA - kB) <= accurasy) {
            point_t pt(NAN, NAN);
            return pt;
        }

        // not parallel or equal lines
        double x0 = (coeffs.b * line.coeffs.c - line.coeffs.b * coeffs.c) / (coeffs.a * line.coeffs.b - line.coeffs.a * coeffs.b);
        double y0 = (-coeffs.a * x0 - coeffs.c) / coeffs.b;
        point_t pt(x0, y0);
        return pt;
    };
};*/

class plate_t {
    vector_t n;
    point_t pt;
    double d;

    public:
    plate_t(const point_t& firstPt, const point_t& secondPt, const point_t& therdPt) : n{NAN, NAN, NAN}, pt{NAN, NAN, NAN}{
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
        n.copy(norm);
        pt.copy(firstPt);
        d = -firstVec.scalarMult(n);
    }
};