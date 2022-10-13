#include "matrix.h"
#include <iostream>
#include <vector>

int main() {
    std::pair<unsigned, std::vector<double>> pr = matrix::get_data();

    matrix::matrix_t matrix(pr.second, pr.first);

    double res = matrix.determinant();

    std::cout << res << std::endl;
}