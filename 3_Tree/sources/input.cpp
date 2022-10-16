#include <vector>
#include <iostream>
#include "input.h"

namespace {

void cls() {
    std::cout << "Incorrect input" << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

}

namespace get_commands {

std::vector<int> get_commands() {

    std::vector<int> commands;

    bool is_end = false;
    while ((!std::cin.eof()) && (!is_end)) {
        char command = 0;
        
        std::cin >> command;
        while(!std::cin) {
            cls();
            std::cin >> command;
        }

        switch (command) {
            case 'k': {
                commands.push_back(static_cast<int>(command_codes::K));

                int val = 0;
                std::cin >> val;
                while(!std::cin) {
                    cls();
                    std::cin >> val;
                }

                commands.push_back(val);
                break;
            }

            case 'q': {
                commands.push_back(static_cast<int>(command_codes::Q));

                int l_border = 0, r_border = 0;
                std::cin >> l_border >> r_border;
                while(!std::cin) {
                    cls();
                    std::cin >> l_border >> r_border;
                }

                commands.push_back(l_border);
                commands.push_back(r_border);
                break;
            }

            case 'n': {
                commands.push_back(static_cast<int>(command_codes::N));

                int border = 0;
                std::cin >> border;
                while(!std::cin) {
                    cls();
                    std::cin >> border;
                }

                commands.push_back(border);
                break;
            }

            case 'm': {
                commands.push_back(static_cast<int>(command_codes::M));

                int num = 0;
                std::cin >> num;
                while(!std::cin) {
                    cls();
                    std::cin >> num;
                }

                commands.push_back(num);
                break;
            }

            default:
                is_end = true;
        }
    }

    return commands;
}

} // get_commands