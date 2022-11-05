#pragma once

#include "triangle_t.h"
#include "bsp.h"
#include <iostream>
#include <vector>
#include <list>

namespace Triangles {

int triagIntersections(const unsigned n);
void getData(std::vector<triangle_info_t>& triangles);
}