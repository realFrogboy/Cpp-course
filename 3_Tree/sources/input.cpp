#include <vector>
#include <iostream>
#include "input.h"
#include <limits>

namespace {

void cls() {
    std::cout << "Incorrect input" << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

template <typename T>
T get() {
    T val;
    std::cin >> val;
    while(!std::cin) {
        cls();
        std::cin >> val;
    }
    return val;
}

}

namespace get_commands {

std::pair<std::vector<char>, std::vector<int>> get_commands() {

    std::vector<char> commands;
    std::vector<int> numbers; 

    bool is_end = false;
    while ((!std::cin.eof()) && (!is_end)) {
        char command = get<char>();
        commands.push_back(command);

        if (command == 'q') {

            int l_border = get<int>();
            int r_border = get<int>();

            numbers.push_back(l_border);
            numbers.push_back(r_border);

        } else if ((command == 'k') || (command == 'n') || (command == 'm')) {

            int val = get<int>();
            numbers.push_back(val);

        } else {
            is_end = true;
        }
    }

    return {commands, numbers};
}

} // get_commands