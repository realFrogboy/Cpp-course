#include "matrix.h"
#include <iostream>
#include "assert.h"

namespace matrix {

std::pair<unsigned, std::vector<double>> get_data() {
    unsigned rank = 0;
    std::cin >> rank;
    assert(std::cin);

    std::vector<double> input(rank * rank);

    for (auto& elem : input) {
        std::cin >> elem;
        assert(std::cin);
    }

    std::pair<unsigned, std::vector<double>> pr = std::make_pair(rank, input);
    return pr;
}

} // matrix