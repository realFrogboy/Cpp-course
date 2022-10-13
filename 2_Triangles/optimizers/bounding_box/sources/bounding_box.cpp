#include "bounding_box.h"

namespace bounding_box {

point_t getMin(const triangle_t& triag) {
    Geometric::trianglePt_t triagPt = triag.trianglePt;

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

point_t getMax(const triangle_t& triag) {
    Geometric::trianglePt_t triagPt = triag.trianglePt;

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

AABB::AABB(const triangle_t& triag) {
    max = getMax(triag);
    min = getMin(triag);
}

bool AABB::isIntersect(const AABB& box) const {
    coord_t coordMax1 = max.coord;
    coord_t coordMin1 = min.coord;
    coord_t coordMax2 = box.max.coord;
    coord_t coordMin2 = box.min.coord;

    if (coordMax1.x < coordMin2.x || coordMin1.x > coordMax2.x) return false;
    if (coordMax1.y < coordMin2.y || coordMin1.y > coordMax2.y) return false;
    if (coordMax1.z < coordMin2.z || coordMin1.z > coordMax2.z) return false;
    
    return true;
}

} // bounding_box