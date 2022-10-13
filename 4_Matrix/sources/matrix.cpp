#include "matrix.h"
#include <iostream>
#include "assert.h"

namespace matrix {

std::pair<unsigned, std::vector<double>> get_data() {
    unsigned rank = 0;
    assert(std::cin >> rank);

    std::vector<double> input(rank * rank);

    for (auto& elem : input)
        assert(std::cin >> elem);

    std::pair<unsigned, std::vector<double>> pr = std::make_pair(rank, input);
    return pr;
}

}