#include "matrix.h"
#include "test_generator.h"

namespace test_generator {

const unsigned num_tests = 1;

int all_random(std::ofstream& fp, const unsigned n) {
    const int xmin = -5;
    const int xmax = 5;

    double det = 1;
    std::vector<double> data;

    for (unsigned i = 0; i < n; i++) {
        for (unsigned null_cnt = 0; null_cnt < i; null_cnt++)
            data.push_back(0);

        double s = 0;
        (i == 0) ? s = 42 : s = 1;
            data.push_back(s);

        det *= s;

        for (unsigned j = i + 1; j < n; j++) {
            int s = (double)rand() / RAND_MAX * (xmax-xmin) + xmin;
            data.push_back(s);
        }       
    }

    matrix::matrix_t matrix(data, n);

    for (int cnt = 0; cnt < 50; cnt++) {
        unsigned lhs = 1 + (unsigned)rand() % n;
        unsigned rhs = 1 + (unsigned)rand() % n;

        bool is_swap = matrix.row_swap(lhs, rhs);
        if (is_swap)
            det *= -1;
    }

    for (int cnt = 0; cnt < 50; cnt++) {
        unsigned lhs = 1 + (unsigned)rand() % n;
        unsigned rhs = 1 + (unsigned)rand() % n;

        bool is_swap = matrix.con_swap(lhs, rhs);
        if (is_swap)
            det *= -1;
    }

    fp << n << std::endl;

    for (unsigned r_idx = 1; r_idx <= n; r_idx++) {
        for (unsigned l_idx = 1; l_idx <= n; l_idx++)
            fp << matrix[r_idx][l_idx] << ' ';
        fp << std::endl;
    }


    fp << det << std::endl << std::endl;

    return 0;
}

void test_generator(const unsigned rang) {
    srand(time(NULL));

    std::ofstream fp("Generation.txt");
    
    for (unsigned cnt = 0; cnt < num_tests; cnt++)
        all_random(fp, rang);
}

} // test_generator