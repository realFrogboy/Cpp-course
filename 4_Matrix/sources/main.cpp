#include "matrix.h"
#include "input.h"
#include <iostream>
#include <vector>

int main() {
    double res = 0;
    try {
        std::pair<unsigned, std::vector<double>> pr = get_data::get_data();
        matrix::matrix_t matrix(pr.second, pr.first);
        res = matrix.determinant();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    std::cout << res << std::endl;
}