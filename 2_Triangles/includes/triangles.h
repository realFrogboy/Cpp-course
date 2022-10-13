#pragma once

#include "triangle_t.h"
#include "bsp.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <list>

namespace Triangles {
int triagIntersections(const unsigned n);
void getData(std::vector<std::pair<Triangles::Geometric::triangle_t, unsigned>>& triangles);
}