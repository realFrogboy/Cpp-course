#include "tree_t.h"
#include "range_queries.h"
#include "input.h"
#include <utility>
#include <iostream>


int main() {
    auto pr = get_commands::get_commands();

    std::vector<int> res = range_queries::range_queries(pr);

    for (auto num : res)
        std::cout << num << std::endl;
}