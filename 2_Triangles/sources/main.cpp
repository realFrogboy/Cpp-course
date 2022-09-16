#include "triangles.h"
#include <stdio.h>

int main() {
    point_t pt1(1.5, -4, 8.25);
    point_t pt2(7, 10, -4.25);
    point_t pt3(5, 3, -0.15);

    point_t pt4(-30.1, 2.45, -10.5);
    point_t pt5(9.23, -6.437, 5.32);
    point_t pt6(-7.453, -2.345, 10.4386);

    plate_t plt1(pt1, pt2, pt3);
    plate_t plt2(pt4, pt5, pt6);

    line_t line1 = plt1.intsectOf2Plt(plt2);
    lineData_t lineData = line1.getData();

    coord_t ptCoord = lineData.linePt.getCoord();
    printf("%lf, %lf, %lf\n", ptCoord.x, ptCoord.y, ptCoord.z);
    return 0;
}