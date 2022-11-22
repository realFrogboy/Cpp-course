#include "matrix.h"
#include "input.h"
#include <iostream>
#include <vector>

int main() {
    std::pair<unsigned, std::vector<double>> pr = get_data::get_data();
    double res = 0;

    try {
        matrix::matrix_t matrix(pr.second, pr.first);
        res = matrix.determinant();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << res << std::endl;
}