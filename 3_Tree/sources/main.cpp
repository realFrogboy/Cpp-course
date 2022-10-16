#include "node_t.h"
#include "tree_t.h"
#include "range_queries.h"
#include "input.h"
#include <iostream>


int main() {
    std::vector<int> commands = get_commands::get_commands();

    std::vector<int> res = range_queries::range_queries(commands);

    for (auto num : res)
        std::cout << num << std::endl;
}