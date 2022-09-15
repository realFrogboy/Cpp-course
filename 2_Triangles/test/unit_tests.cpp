#include "triangles.h"
#include <gtest/gtest.h>

// VECTOR TESTS
TEST(vectorTest, Initialization) {
    vector_t vec(2, -6, 8);
    coord_t vecCoord = vec.getCoord();

    EXPECT_EQ(1, isEqual(2, vecCoord.x));
    EXPECT_EQ(1, isEqual(-6, vecCoord.y));
    EXPECT_EQ(1, isEqual(8, vecCoord.z));
}

TEST(vectorTest, Copy) {
    vector_t vec1(1.5, -4, 8.25);
    vector_t vec2(NAN, NAN, NAN);
    vector_t vec3(0, 0, 0);

    vec2.copy(vec1);
    EXPECT_EQ(1, vec1.isEqual(vec2));

    vec3.copy(vec2);
    EXPECT_EQ(1, vec3.isEqual(vec1));
}

TEST(vectorTest, ConstMult) {
    vector_t vec1(1.5, -4, 8.25);
    double coeff = -0.25;

    vector_t res = vec1 * coeff;
    vector_t trueRes(-0.375, 1, -2.0625);
    EXPECT_EQ(1, res.isEqual(trueRes));
    
    res = res * -4;
    EXPECT_EQ(1, res.isEqual(vec1));

    res = res * 0;
    vector_t nullVec(0, 0, 0);
    EXPECT_EQ(1, res.isEqual(nullVec));
}

TEST(vectorTest, ScalarMult) {
    vector_t vec1(1.5, -4, 8.25);
    vector_t vec2(7, 10, -4.25);
    vector_t vec3(5, 3, -0.15);
    
    double mult = vec1.scalarMult(vec2);
    EXPECT_EQ(1, isEqual(-64.5625, mult));

    mult = vec1.scalarMult(vec3);
    EXPECT_EQ(1, isEqual(-5.7375, mult));

    mult = vec2.scalarMult(vec3);
    EXPECT_EQ(1, isEqual(65.6375, mult));

    mult = vec1.scalarMult(vec1);
    EXPECT_EQ(1, isEqual(86.3125, mult));
    EXPECT_EQ(1, isEqual(86.3125, vec1.length() * vec1.length()));

    vector_t nullVec(0, 0, 0);
    vector_t norm = vec1.normal();
    mult = vec1.scalarMult(norm);
    EXPECT_EQ(1, isEqual(0, mult));
}

TEST(vectorTest, VectorMult) {
    vector_t vec1(1.5, -4, 8.25);
    vector_t vec2(7, 10, -4.25);
    vector_t vec3(5, 3, -0.15);

    vector_t res = vec1.vectorMult(vec2);
    vector_t trueRes1(-65.5, 64.125, 43);
    EXPECT_EQ(1, res.isEqual(trueRes1));

    res = vec1.vectorMult(vec3);
    vector_t trueRes2(-24.15, 41.475, 24.5);
    EXPECT_EQ(1, res.isEqual(trueRes2));

    res = vec2.vectorMult(vec3);
    vector_t trueRes3(11.25, -20.2, -29);
    EXPECT_EQ(1, res.isEqual(trueRes3));

    res = vec1.vectorMult(vec1);
    vector_t trueRes4(0, 0, 0);
    EXPECT_EQ(1, res.isEqual(trueRes4));
}

TEST(vectorTest, vectorAddition) {
    vector_t vec1(1.5, -4, 8.25);
    vector_t vec2(7, 10, -4.25);
    vector_t vec3(5, 3, -0.15);

    vector_t res = vec1 + vec2 + vec3;
    vector_t trueRes(13.5, 9, 3.85);
    EXPECT_EQ(1, res.isEqual(trueRes));
}

TEST(vectorTest, vectorDifference) {
    vector_t vec1(1.5, -4, 8.25);
    vector_t vec2(7, 10, -4.25);
    vector_t vec3(5, 3, -0.15);

    vector_t res = vec1 - vec2 - vec3;
    vector_t trueRes(-10.5, -17, 12.65);
    EXPECT_EQ(1, res.isEqual(trueRes));
}

TEST(vectorTest, Length) {
    vector_t vec1(1.5, -4, 8.25);
    vector_t vec2(7, 10, -4.25);
    vector_t vec3(5, 3, -0.15);

    double len = vec1.length();
    EXPECT_EQ(1, isEqual(len, 9.290452088));
    EXPECT_EQ(1, isEqual(len * len, vec1.scalarMult(vec1)));

    len = vec2.length();
    EXPECT_EQ(1, isEqual(len, 12.9252659));
    EXPECT_EQ(1, isEqual(len * len, vec2.scalarMult(vec2)));

    len = vec3.length();
    EXPECT_EQ(1, isEqual(len, 5.8328809));
    EXPECT_EQ(1, isEqual(len * len, vec3.scalarMult(vec3)));
}


// POINT TEST
TEST(pointTest, Initialization) {
    point_t pt(2, -6, 8);
    coord_t ptCoord = pt.getCoord();

    EXPECT_EQ(1, isEqual(2, ptCoord.x));
    EXPECT_EQ(1, isEqual(-6, ptCoord.y));
    EXPECT_EQ(1, isEqual(8, ptCoord.z));
}

TEST(pointTest, Copy) {
    point_t pt1(1.5, -4, 8.25);
    point_t pt2(NAN, NAN, NAN);
    point_t pt3(0, 0, 0);

    pt2.copy(pt1);
    EXPECT_EQ(1, pt1.isEqual(pt2));

    pt3.copy(pt2);
    EXPECT_EQ(1, pt3.isEqual(pt1));
}

TEST(pointTest, ToVector) {
    point_t pt(1.5, -4, 8.25);

    vector_t vec = pt.toVector();
    vector_t trueVec(1.5, -4, 8.25);

    EXPECT_EQ(1, vec.isEqual(trueVec));
}


// LINE TEST
TEST(lineTest, Initialization) {
    point_t pt1(1.5, -4, 8.25);
    point_t pt2(7, 10, -4.25);
    point_t pt3(5, 3, -0.15);

    line_t line1(pt1, pt2);
    lineData_t lineData = line1.getData();
    vector_t dir1(5.5, 14, -12.5);
    EXPECT_EQ(1, dir1.isParallel(lineData.a));
    EXPECT_EQ(1, pt1.isEqual(lineData.linePt));

    line_t line2(pt1, pt3);
    lineData = line2.getData();
    vector_t dir2(3.5, 7, -8.4);
    EXPECT_EQ(1, dir2.isParallel(lineData.a));
    EXPECT_EQ(1, pt1.isEqual(lineData.linePt));

    line_t line3(pt3, pt2);
    lineData = line3.getData();
    vector_t dir3(-2, -7, 4.1);
    EXPECT_EQ(1, dir3.isParallel(lineData.a));
    EXPECT_EQ(1, pt3.isEqual(lineData.linePt));
}

