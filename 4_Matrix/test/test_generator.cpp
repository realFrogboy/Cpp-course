#include "matrix.h"
#include "test_generator.h"
#include <exception>


namespace test_generator {

constexpr unsigned num_tests = 100;

int random_matrix(std::ofstream& fp, const unsigned n) {

    double det = 1;
    std::vector<double> data;

    for (unsigned i = 0; i < n; ++i) {
        for (unsigned null_cnt = 0; null_cnt < i; null_cnt++)
            data.push_back(0);

        double s = 0;
        (i == 0) ? s = 42 : s = 1;
            data.push_back(s);

        det *= s;

        for (unsigned j = i + 1; j < n; ++j) {
            data.push_back(1);
        }       
    }

    matrix::matrix_t matrix(data, n);

    for (int cnt = 0; cnt < 400; ++cnt) {
        unsigned lhs = 1 + (unsigned)rand() % n;
        unsigned rhs = 1 + (unsigned)rand() % n;

        if (lhs == rhs) continue;

        matrix::matrix_t::proxy_row row_l = matrix[lhs];
        matrix::matrix_t::proxy_row row_r = matrix[rhs];

        row_l += row_r;
    }

    for (unsigned cnt = 0; cnt < 200; ++cnt) {
        unsigned lhs = 1 + (unsigned)rand() % n;
        unsigned rhs = 1 + (unsigned)rand() % n;

        bool is_swap = matrix.con_swap(lhs, rhs);
        if (is_swap)
            det *= -1;
    }

    for (int cnt = 0; cnt < 400; ++cnt) {
        unsigned lhs = 1 + (unsigned)rand() % n;
        unsigned rhs = 1 + (unsigned)rand() % n;

        if (lhs == rhs) continue;

        matrix::matrix_t::proxy_row row_l = matrix[lhs];
        matrix::matrix_t::proxy_row row_r = matrix[rhs];

        row_l -= row_r;
    }

    fp << n << std::endl;

    for (unsigned r_idx = 1; r_idx <= n; ++r_idx) {
        for (unsigned l_idx = 1; l_idx <= n; ++l_idx)
            fp << matrix[r_idx][l_idx] << ' ';
        fp << std::endl;
    }


    fp << det << std::endl << std::endl;

    return 0;
}

void test_generator(const unsigned rank) {
    std::ofstream fp("Generation.txt");
    if (!fp.is_open()) {
        throw std::runtime_error("failed to open file!");
    }
    
    for (unsigned cnt = 0; cnt < num_tests; ++cnt)
        random_matrix(fp, rank);
}

} // test_generator