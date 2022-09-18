#include "triangles.h"

static point_t getPoint() {
    double x = 0, y = 0, z = 0;
    int tmp = scanf("%lf %lf %lf", &x, &y, &z);
    assert(tmp == 3);

    point_t pt(x, y, z);
    return pt;
}

int triagIntersections(const unsigned n) {
    list<pair<triangle_t, unsigned>> triangles;

    for (unsigned idx = 0; idx < n; idx++) {
        point_t pt1 = getPoint();
        point_t pt2 = getPoint();
        point_t pt3 = getPoint();

        triangle_t triag(pt1, pt2, pt3);

        auto iter = triangles.begin();
        for (; iter != triangles.end(); ++iter) {
            pair<triangle_t, unsigned> pr = *iter;
            if (pr.first.isIntersection3D(triag)) {
                printf("%d %d\n", pr.second, idx);
                triangles.erase(iter);
                break;
            }
        }

        if (iter == triangles.end()) {
            pair<triangle_t, unsigned> pr = make_pair(triag, idx);
            triangles.push_back(pr);
        }
    }
    return 0;
}