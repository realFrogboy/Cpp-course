#include "triangles.h"

namespace Triangles {

namespace {

Geometric::point_t getPoint() {
    double x = 0, y = 0, z = 0;
    int tmp = scanf("%lf %lf %lf", &x, &y, &z);
    assert(tmp == 3);

    Geometric::point_t pt(x, y, z);
    return pt;
}

} // namespace

int triagIntersections(const unsigned n) {
    std::list<std::pair<Geometric::triangle_t, unsigned>> uncrossedTriangles;
    std::vector<std::pair<Geometric::triangle_t, unsigned>> crossedTriangles;

    for (unsigned idx = 0; idx < n; idx++) {
        Geometric::point_t pt1 = getPoint();
        Geometric::point_t pt2 = getPoint();
        Geometric::point_t pt3 = getPoint();

        Geometric::triangle_t triag(pt1, pt2, pt3);
        std::pair<Geometric::triangle_t, unsigned> newPr = std::make_pair(triag, idx);

        bool fl = false;
        for (auto lstIter = uncrossedTriangles.begin(); lstIter != uncrossedTriangles.end(); ++lstIter) {
            std::pair<Geometric::triangle_t, unsigned> pr = *lstIter;
            if (pr.first.isIntersection3D(triag)) {
                printf("%d ", pr.second);
                fl = true;

                lstIter = uncrossedTriangles.erase(lstIter);
                crossedTriangles.push_back(pr);
            }
        }

        if (fl) {
            printf("%d ", idx);
            crossedTriangles.push_back(newPr);
            continue;
        }

        auto vecIter = crossedTriangles.begin();
        for (; vecIter != crossedTriangles.end(); ++vecIter) {
            std::pair<Geometric::triangle_t, unsigned> pr = *vecIter;
            if (pr.first.isIntersection3D(triag)) {
                printf("%d ", idx);

                crossedTriangles.push_back(newPr);

                break;
            }
        }

        if (vecIter == crossedTriangles.end())
            uncrossedTriangles.push_back(newPr);
    }
    printf("\n");
    return 0;
}

} // Triangles