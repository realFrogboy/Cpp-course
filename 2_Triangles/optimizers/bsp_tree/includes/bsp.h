#pragma once

#include <vector>
#include <utility>
#include <iostream>
#include <unordered_map>
#include "triangle_t.h"
#include "bounding_box.h"
#include <algorithm>

namespace bsp_tree {

const double accurasy = 0.0001;

enum class location {
    COINCIDENT,
    FRONT,
    BACK,
    INTERSECTION
};

using nodeVec = typename std::vector<std::pair<Geometric::triangle_t, unsigned>>;
using Geometric::triangle_t;
using Geometric::plate_t;

location classifyTriangle(const plate_t& plt, const triangle_t& triag);

std::pair<triangle_t, unsigned> getSplit(const nodeVec& arr);

class bspTree_t {

    std::unordered_map<unsigned, unsigned> crossedTriangles;
    std::vector<unsigned> output;

    public:

    bspTree_t(const nodeVec& triangles);

    void print();

    void searchIntersectionsTree(const nodeVec& arr);

    std::pair<nodeVec, nodeVec> locateTriangles(const nodeVec& arr, const plate_t& plt);

    void Intersections(const nodeVec& splitTriangles);
};

} // bsp_tree