#include "help.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <iostream>

namespace lve {

bool help(int argc, char ** argv) {
    const struct option long_option[] = {
        {"help", no_argument, NULL, 'h'}};

	int long_index;
    int opt;
    const char short_option[] = "h";
    
    int flag_h = 0;
    
    
    while((opt = getopt_long(argc, argv, short_option, long_option, &long_index)) != -1) {
        if (opt == 'h') flag_h = 1;
    }
    
    if (!flag_h) return 0;

    std::cout << "Triangle visualization on vulkan API by Daniil Avdeev\n"
                "Keyboard movement information:\n"
                "Key A - movement to the left\n"
                "Key D - movement to the right\n"
                "Key W - moving forward\n"
                "Key S - moving backward\n"
                "Key E - moving up\n"
                "Key Q - moving down" << std::endl;

    return 1;
}

} // lve