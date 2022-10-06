#pragma once

#include <vector>
#include <utility>
#include <iostream>
#include <unordered_map>
#include "triangle_t.h"
#include "bounding_box.h"

namespace bsp_tree {

const double accurasy = 0.0001;

enum class location {
    COINCIDENT,
    FRONT,
    BACK,
    INTERSECTION
};

using nodeVec = typename std::vector<std::pair<Triangles::Geometric::triangle_t, unsigned>>;
using Triangles::Geometric::triangle_t;
using Triangles::Geometric::plate_t;

location classifyTriangle(const plate_t& plt, const triangle_t& triag);

std::pair<triangle_t, unsigned> getSplit(const nodeVec& arr);

class bspTree_t {
    public:

    std::unordered_map<unsigned, unsigned> crossedTriangles;

    inline bspTree_t(const nodeVec& triangles);

    inline void searchIntersectionsTree(const nodeVec& arr);

    inline std::pair<nodeVec, nodeVec> locateTriangles(const nodeVec& arr, const plate_t& plt);

    inline void Intersections(const nodeVec& splitTriangles);
};


inline bspTree_t::bspTree_t(const nodeVec& triangles) {
    searchIntersectionsTree(triangles);
}

inline void bspTree_t::searchIntersectionsTree(const nodeVec& arr) {
    auto currTriangle = getSplit(arr);
    Triangles::Geometric::trianglePt_t triangleVertex = currTriangle.first.trianglePt;

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
        location res = classifyTriangle(plt, triag.first);

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

        bounding_box::AABB box(iter->first);

        for (auto iter1 = std::next(iter); iter1 != splitTriangles.end(); ++iter1) {
            bool fl1 = true;
            auto triangleIter1 = crossedTriangles.find(iter1->second);
            if (triangleIter1 != crossedTriangles.end())
                fl1 = false;

            bounding_box::AABB box1(iter1->first);

            if ((box.isIntersect(box1)) && (iter->first.isIntersection3D(iter1->first))) {
                if (fl1)
                    std::cout << iter1->second << std::endl;
                fl = true;
                crossedTriangles.insert({iter1->second, iter1->second});
            }
        }
        auto triangleIter = crossedTriangles.find(iter->second);
        if (triangleIter != crossedTriangles.end())
            fl = false;

        if (fl) {
            std::cout << iter->second << std::endl;
            crossedTriangles.insert({iter->second, iter->second});
        }
    }
}

} // bsp_tree