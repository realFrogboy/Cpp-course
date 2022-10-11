#include "matrix.h"
#include <assert.h>
#include <iostream>
#include <vector>

int main() {

    std::vector<double> elems{1, 2, 3, 4, 5, 6, 7, 8, 9};
    matrix::matrix_t matrix(elems, 3);

    /*unsigned rang = 0;
    assert(std::cin >> rang);

    std::vector<int> input(rang * rang);
    for (auto& elem : input)
        assert(std::cin >> elem);

    matrix::matrix_t matrix(input, rang);
    matrix::matrix_t::proxy_row::r_rang = rang;

    std::cout << matrix[1][1] << std::endl;
    std::cout << matrix[rang][rang] << std::endl;*/
}