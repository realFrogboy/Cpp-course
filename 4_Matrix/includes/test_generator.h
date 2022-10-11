#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <string>
#include <fstream>

namespace test_generator {

int all_random(std::ofstream& fp, const unsigned n);
void test_generator(const unsigned rang);

}