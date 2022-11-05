#pragma once

#include <vector>
#include <utility>
#include <iostream>
#include <unordered_map>
#include "triangle_t.h"
#include "bounding_box.h"
#include <algorithm>

namespace Triangles {

class triangle_info_t {
    public:
    Geometric::triangle_t triangle;
    unsigned number;
    bool is_intersect;
};

}

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


inline bspTree_t::bspTree_t(const nodeVec& triangles) {
    searchIntersectionsTree(triangles);
}

inline void bspTree_t::print(nodeVec& arr) {
        std::sort(output.begin(), output.end());

        for (auto elem : output) {
            arr[elem].is_intersect = true;
            std::cout << elem << std::endl;
        }
}

inline void bspTree_t::searchIntersectionsTree(const nodeVec& arr) {
    auto currTriangle = getSplit(arr);
    Triangles::Geometric::trianglePt_t triangleVertex = currTriangle.trianglePt;

    if (!triangleVertex.first.isValid()) {
        Intersections(arr);
        return;
    }

    plate_t plt(triangleVertex.first, triangleVertex.second, triangleVertex.third);
    std::pair<nodeVec, nodeVec> pr = locateTriangles(arr, plt);

    if (pr.first.size())
        searchIntersectionsTree(pr.first);

    if (pr.second.size())
        searchIntersectionsTree(pr.second);
}


inline std::pair<nodeVec, nodeVec> bspTree_t::locateTriangles(const nodeVec& arr, const plate_t& plt) {
    nodeVec frontLst, backLst, splitTriangles;

    for (auto triag : arr) {
        location res = classifyTriangle(plt, triag.triangle);

        switch(res) {
            case location::COINCIDENT: 
                splitTriangles.push_back(triag);
                break;

            case location::FRONT:
                frontLst.push_back(triag);
                break;

            case location::BACK:
                backLst.push_back(triag);
                break;

            case location::INTERSECTION:
                splitTriangles.push_back(triag);
                frontLst.push_back(triag);
                backLst.push_back(triag);
        }
    }

    std::pair<nodeVec, nodeVec> pr;

    if ((static_cast<int>(frontLst.size()) - 300 < static_cast<int>(splitTriangles.size())) && (static_cast<int>(backLst.size()) - 300 < static_cast<int>(splitTriangles.size()))) {
        Intersections(arr);
        return pr;
    }

    Intersections(splitTriangles);

    if (!frontLst.empty())
        pr.first = frontLst;
    
    if (!backLst.empty())
        pr.second = backLst;

    return pr;
} 


inline void bspTree_t::Intersections(const nodeVec& splitTriangles) {
    auto end = std::prev(splitTriangles.end());
    for (auto iter = splitTriangles.begin(); iter != end; ++iter) {
        bool fl = false;

        bounding_box::AABB box(iter->triangle);

        for (auto iter1 = std::next(iter); iter1 != splitTriangles.end(); ++iter1) {
            bool fl1 = true;
            auto triangleIter1 = crossedTriangles.find(iter1->number);
            if (triangleIter1 != crossedTriangles.end())
                fl1 = false;

            bounding_box::AABB box1(iter1->triangle);

            if ((box.isIntersect(box1)) && (iter->triangle.isIntersection3D(iter1->triangle))) {
                if (fl1) {
                    output.push_back(iter1->number);
                }
                fl = true;
                crossedTriangles.insert({iter1->number, iter1->number});
            }
        }
        auto triangleIter = crossedTriangles.find(iter->number);
        if (triangleIter != crossedTriangles.end())
            fl = false;

        if (fl) {
            output.push_back(iter->number);
            crossedTriangles.insert({iter->number, iter->number});
        }
    }
}

} // bsp_tree