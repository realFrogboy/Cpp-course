#include "matrix.h"
#include <iostream>
#include "assert.h"

namespace matrix {

std::pair<unsigned, std::vector<double>> get_data() {
    unsigned rang = 0;
    assert(std::cin >> rang);

    std::vector<double> input(rang * rang);

    for (auto& elem : input)
        assert(std::cin >> elem);

    std::pair<unsigned, std::vector<double>> pr = std::make_pair(rang, input);
    return pr;
}

}