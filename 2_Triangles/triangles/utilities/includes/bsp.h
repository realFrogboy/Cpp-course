#pragma once

#include "triangle_t.h"
#include "triangle_info.h"

#include <vector>
#include <utility>
#include <unordered_map>


namespace bsp_tree {

const double accurasy = 0.0001;

enum class location {
    COINCIDENT,
    FRONT,
    BACK,
    INTERSECTION
};

using nodeVec = typename std::vector<Triangles::triangle_info_t>;
using Triangles::Geometric::triangle_t;
using Triangles::Geometric::plate_t;

location classifyTriangle(const plate_t& plt, const triangle_t& triag);

triangle_t getSplit(const nodeVec& arr);

class bspTree_t {

    std::unordered_map<unsigned, unsigned> crossedTriangles;
    std::vector<unsigned> output;

    public:

    bspTree_t(const nodeVec& triangles);

    void print(nodeVec& arr);

    void searchIntersectionsTree(const nodeVec& arr);

    std::pair<nodeVec, nodeVec> locateTriangles(const nodeVec& arr, const plate_t& plt);

    void Intersections(const nodeVec& splitTriangles);
};

} // bsp_tree