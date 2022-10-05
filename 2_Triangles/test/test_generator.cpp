#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <string>
#include <fstream>

namespace {

int all_random(std::ofstream& fp, const int n) {
    const double xmin = -1000.0;
    const double xmax = 1000.0;
    const int pointsInTriangle = 9;

    fp << n << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < pointsInTriangle; j++) {
            double s = (double)rand() / RAND_MAX * (xmax-xmin) + xmin;
            fp << s << ' ';
        }

        fp << std::endl;
    }

    return 0;
}

} // namespace

int main(int argc, char **argv) {
    srand(time(NULL));

    assert(argc == 2);

    char *nStr = argv[1];
    int n = std::stoi(nStr);

    std::ofstream fp("Generation.txt");

    all_random(fp, n);
}

