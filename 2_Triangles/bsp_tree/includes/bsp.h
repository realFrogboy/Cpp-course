#pragma once

#include <list>
#include <utility>
#include <iostream>
#include <unordered_map>
#include "triangle_t.h"

namespace bsp_tree {

const double accurasy = 0.0001;

enum class location {
    COINCIDENT,
    FRONT,
    BACK,
    INTERSECTION
};

location classifyTriangle(const Triangles::Geometric::plate_t& plt, const Triangles::Geometric::triangle_t& triag);

std::pair<Triangles::Geometric::triangle_t, unsigned> getSplit(const std::list<std::pair<Triangles::Geometric::triangle_t, unsigned>>& triangles);

class bspNode_t {
    public:

    std::list<std::pair<Triangles::Geometric::triangle_t, unsigned>> splitTriangles;
    bspNode_t *ptrFront = nullptr;
    bspNode_t *ptrBack = nullptr;


    inline void nodeCtor(const std::list<std::pair<Triangles::Geometric::triangle_t, unsigned>>& triangles);

    inline void nodeDtor();
};


class bspTree_t {
    public:

    bspNode_t* root;
    std::unordered_map<unsigned, unsigned> crossedTriangles;


    bspTree_t(bspNode_t node) : root(&node) {};

    inline bspTree_t(const std::list<std::pair<Triangles::Geometric::triangle_t, unsigned>>& triangles);

    inline void Intersections(const bspNode_t* node);

    inline void inorder(const bspNode_t* node);

    inline void deleteTree();
};


inline bspTree_t::bspTree_t(const std::list<std::pair<Triangles::Geometric::triangle_t, unsigned>>& triangles) {
    root = new bspNode_t {};
    (*root).nodeCtor(triangles);
}

inline void bspTree_t::inorder(const bspNode_t* node) {
    if (!node)
        return;

    inorder(node->ptrFront);

    Intersections(node);

    inorder(node->ptrBack);
}

inline void bspTree_t::deleteTree() {
    root->nodeDtor();
}


inline void bspNode_t::nodeCtor(const std::list<std::pair<Triangles::Geometric::triangle_t, unsigned>>& triangles) {
    auto currTriangle = getSplit(triangles);
    Triangles::Geometric::trianglePt_t triangleVertex = currTriangle.first.getData();

    if (!triangleVertex.first.isValid()) {
        for (auto iter = triangles.begin(); iter != triangles.end(); ++iter)
            splitTriangles.push_back(*iter);

        return;
    }

    splitTriangles.push_back(currTriangle);


    Triangles::Geometric::plate_t plt(triangleVertex.first, triangleVertex.second, triangleVertex.third);

    std::list<std::pair<Triangles::Geometric::triangle_t, unsigned>> frontLst, backLst;

    auto iter = triangles.begin();
    ++iter;
    for (; iter != triangles.end(); ++iter) {
        location res = classifyTriangle(plt, iter->first);

        switch(res) {
            case location::COINCIDENT: 
                splitTriangles.push_back(*iter);
                break;

            case location::FRONT:
                frontLst.push_back(*iter);
                break;

            case location::BACK:
                backLst.push_back(*iter);
                break;

            case location::INTERSECTION:
                splitTriangles.push_back(*iter);
                frontLst.push_back(*iter);
                backLst.push_back(*iter);
        }
    }

    if (!frontLst.empty()) {
        ptrFront = new bspNode_t {};
        (*ptrFront).nodeCtor(frontLst);
    }

    if(!backLst.empty()) {
        ptrBack = new bspNode_t {};
        (*ptrBack).nodeCtor(backLst);
    }
}

inline void bspNode_t::nodeDtor() {
    if (ptrFront)
        (*ptrFront).nodeDtor();

    if (ptrBack)
        (*ptrBack).nodeDtor();

    delete this;
    return;
}


inline void bspTree_t::Intersections(const bspNode_t* node) {
    auto end = node->splitTriangles.end();
    --end;
    for (auto iter = node->splitTriangles.begin(); iter != end; ++iter) {
        bool fl = false;

        for (auto iter1 = std::next(iter,1); iter1 != node->splitTriangles.end(); ++iter1) {
            bool fl1 = true;
            auto triangleIter1 = crossedTriangles.find(iter1->second);
            if (triangleIter1 != crossedTriangles.end())
                fl1 = false;

            if (iter->first.isIntersection3D(iter1->first)) {
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