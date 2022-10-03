#include "bounding_box.h"

namespace bounding_box {

Triangles::Geometric::point_t getMin(const Triangles::Geometric::triangle_t& triag) {
    Triangles::Geometric::trianglePt_t triagPt = triag.getData();

    Triangles::Geometric::coord_t coordPt1 = triagPt.first.getCoord();
    Triangles::Geometric::coord_t coordPt2 = triagPt.second.getCoord();
    Triangles::Geometric::coord_t coordPt3 = triagPt.third.getCoord();

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

    Triangles::Geometric::point_t res {minX, minY, minZ};
    return res;
}

Triangles::Geometric::point_t getMax(const Triangles::Geometric::triangle_t& triag) {
    Triangles::Geometric::trianglePt_t triagPt = triag.getData();

    Triangles::Geometric::coord_t coordPt1 = triagPt.first.getCoord();
    Triangles::Geometric::coord_t coordPt2 = triagPt.second.getCoord();
    Triangles::Geometric::coord_t coordPt3 = triagPt.third.getCoord();

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

    Triangles::Geometric::point_t res {maxX, maxY, maxZ};
    return res;
}

} // bounding_box