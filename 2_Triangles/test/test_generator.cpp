#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <string>

namespace {

int all_random(FILE *fp, int n) {
    const double xmin = -1000.0;
    const double xmax = 1000.0;
    const int pointsInTriangle = 9;

    fprintf(fp, "%d\n", n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < pointsInTriangle; j++) {
            double s=(double)rand()/RAND_MAX*(xmax-xmin)+xmin;
            fprintf(fp, "%lf ", s);
        }

        fprintf(fp, "\n");
    }

    return 0;
}

} // namespace

int main(int argc, char **argv) {
    srand(time(NULL));

    assert(argc == 2);

    char *nStr = argv[1];
    int n = std::stoi(nStr);

    FILE *fp = fopen("Generation.txt", "w");
    assert(fp);

    all_random(fp, n);

    fclose(fp);
}

