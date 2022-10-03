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
using triangle_type = Triangles::Geometric::triangle_t;

location classifyTriangle(const Triangles::Geometric::plate_t& plt, const Triangles::Geometric::triangle_t& triag);

std::pair<triangle_type, unsigned> getSplit(const std::pair<triangle_type, unsigned>* arr, int n);

void toArr(std::pair<triangle_type, unsigned>* arr, const nodeVec& vector);


class bspTree_t {
    public:

    std::unordered_map<unsigned, unsigned> crossedTriangles;

    inline bspTree_t(const nodeVec& triangles);

    inline void searchIntersectionsTree(const std::pair<triangle_type, unsigned>* arr, int n);

    inline std::tuple<std::pair<triangle_type, unsigned>*, std::pair<triangle_type, unsigned>*, int, int> locateTriangles(const std::pair<triangle_type, unsigned>* arr, const int n, const Triangles::Geometric::plate_t& plt);

    inline void Intersections(const nodeVec& splitTriangles);
};


inline bspTree_t::bspTree_t(const nodeVec& triangles) {
    std::pair<triangle_type, unsigned> *arr = new std::pair<triangle_type, unsigned> [triangles.size()];
    toArr(arr, triangles);
    searchIntersectionsTree(arr, triangles.size());
}

inline void bspTree_t::searchIntersectionsTree(const std::pair<triangle_type, unsigned>* arr, int n) {
    auto currTriangle = getSplit(arr, n);
    Triangles::Geometric::trianglePt_t triangleVertex = currTriangle.first.getData();

    if (!triangleVertex.first.isValid()) {
        nodeVec splitTriangles;
        for (int idx = 0; idx < n; idx++)
            splitTriangles.push_back(arr[idx]);

        Intersections(splitTriangles);
        return;
    }

    Triangles::Geometric::plate_t plt(triangleVertex.first, triangleVertex.second, triangleVertex.third);
    std::tuple<std::pair<triangle_type, unsigned>*, std::pair<triangle_type, unsigned>*, int, int> tpl = locateTriangles(arr, n, plt);
    delete[] arr;

    if (std::get<0>(tpl) != nullptr)
        searchIntersectionsTree(std::get<0>(tpl), std::get<2>(tpl));

    if (std::get<1>(tpl) != nullptr)
        searchIntersectionsTree(std::get<1>(tpl), std::get<3>(tpl));
}


inline std::tuple<std::pair<triangle_type, unsigned>*, std::pair<triangle_type, unsigned>*, int, int> bspTree_t::locateTriangles(const std::pair<triangle_type, unsigned>* arr, const int n, const Triangles::Geometric::plate_t& plt) {
    nodeVec frontLst, backLst, splitTriangles, allTriangles;

    for (int idx = 0; idx < n; idx++) {
        location res = classifyTriangle(plt, arr[idx].first);
        allTriangles.push_back(arr[idx]);

        switch(res) {
            case location::COINCIDENT: 
                splitTriangles.push_back(arr[idx]);
                break;

            case location::FRONT:
                frontLst.push_back(arr[idx]);
                break;

            case location::BACK:
                backLst.push_back(arr[idx]);
                break;

            case location::INTERSECTION:
                splitTriangles.push_back(arr[idx]);
                frontLst.push_back(arr[idx]);
                backLst.push_back(arr[idx]);
        }
    }

    std::tuple<std::pair<triangle_type, unsigned>*, std::pair<triangle_type, unsigned>*, int, int> tpl = std::make_tuple(nullptr, nullptr, 0, 0);

    if ((static_cast<int>(frontLst.size()) - 300 < static_cast<int>(splitTriangles.size())) && (static_cast<int>(backLst.size()) - 300 < static_cast<int>(splitTriangles.size()))) {
        Intersections(allTriangles);
        return tpl;
    }

    Intersections(splitTriangles);

    if (!frontLst.empty()) {
        std::pair<triangle_type, unsigned> *aFront = new std::pair<triangle_type, unsigned> [frontLst.size()];
        toArr(aFront, frontLst);
        std::get<0>(tpl) = aFront;
        std::get<2>(tpl) = frontLst.size(); 
    }
    
    if (!backLst.empty()) {
        std::pair<triangle_type, unsigned> *aBack = new std::pair<triangle_type, unsigned> [backLst.size()];
        toArr(aBack, backLst);
        std::get<1>(tpl) = aBack;
        std::get<3>(tpl) = backLst.size();
    }

    return tpl;
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
                    std::cout << iter1->second << "\n";
                fl = true;
                crossedTriangles.insert({iter1->second, iter1->second});
            }
        }
        auto triangleIter = crossedTriangles.find(iter->second);
        if (triangleIter != crossedTriangles.end())
            fl = false;

        if (fl) {
            std::cout << iter->second << "\n";
            crossedTriangles.insert({iter->second, iter->second});
        }
    }
}

} // bsp_tree