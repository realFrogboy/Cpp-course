#include "triangles.h"

int main() {
    unsigned n = 0;
    int tmp = scanf("%u", &n);
    assert(tmp == 1);

    Triangles::triagIntersections(n);

    return 0;
}