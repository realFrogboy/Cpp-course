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
    vector_t norm = vec1.normal1();
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
TEST(lineTest, InitializationBy2Points) {
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

TEST(lineTest, OnLine) {
    point_t pt1(1.5, -4, 8.25);
    point_t pt2(7, 10, -4.25);

    line_t line1(pt1, pt2);
    EXPECT_EQ(1, line1.onLine(pt1));

    point_t pt4(-17.13186, -51.42656, 50.59514);
    EXPECT_EQ(1, line1.onLine(pt4));

    point_t pt5(-7.95989, -28.07972, 29.74975);
    EXPECT_EQ(1, line1.onLine(pt5));
}

TEST(lineTest, GettingT) {
    point_t pt1(1.5, -4, 8.25);
    point_t pt2(7, 10, -4.25);

    line_t line1(pt1, pt2);

    double t = line1.getT(pt1);
    EXPECT_EQ(1, isEqual(t, 0));

    t = line1.getT(pt2);
    EXPECT_EQ(1, isEqual(t, 1));

    point_t pt4(-6.75, -7, 31.25);
    t = line1.getT(pt4);
    EXPECT_EQ(1, isEqual(t, -1.5));

    point_t pt5(12.5, 24, 16.75);
    t = line1.getT(pt5);
    EXPECT_EQ(1, isEqual(t, 2));
}

TEST(lineTest, ProjectionOfPoint) {
    point_t pt1(1.5, -4, 8.25);
    point_t pt2(7, 10, -4.25);
    point_t pt3(5, 3, -0.15);

    line_t line1(pt1, pt2);

    point_t res1 = line1.projOfPoint(pt3);
    point_t trueRes1(4.695752, 4.134641, 0.986928);
    EXPECT_EQ(1, res1.isEqual(trueRes1));

    line_t line2(pt2, pt3);

    point_t res2 = line2.projOfPoint(pt1);
    point_t trueRes2(2.408967, -6.068615, 5.161617);
    EXPECT_EQ(1, res2.isEqual(trueRes2));

    line_t line3(pt3, pt1);

    point_t res3 = line3.projOfPoint(pt2);
    point_t trueRes3(7.401487, 7.802974, -5.913569);
    EXPECT_EQ(1, res3.isEqual(trueRes3));
}


// PLATE TEST
TEST(plateTest, InitializationBy3Points) {
    point_t pt1(1.5, -4, 8.25);
    point_t pt2(7, 10, -4.25);
    point_t pt3(5, 3, -0.15);
    point_t pt4(-30.1, 2.45, -10.5);

    plate_t plt1(pt1, pt2, pt3);

    plateData_t plt1Data = plt1.getData();
    coord_t n1Coord = plt1Data.n.getCoord();
    vector_t trueVec1(-30.1, 2.45, -10.5);
    double k1 = abs(n1Coord.x / -30.1);
    EXPECT_EQ(1, plt1Data.n.isParallel(trueVec1));
    EXPECT_EQ(1, isEqual(plt1Data.d, k1 * 141.575));

    plate_t plt2(pt1, pt2, pt4);
    plateData_t plt2Data = plt2.getData();
    coord_t n2Coord = plt2Data.n.getCoord();
    vector_t trueVec2(-181.875, 498.125, 477.875);
    double k2 = abs(n2Coord.x / -181.875);
    EXPECT_EQ(1, plt2Data.n.isParallel(trueVec2));
    EXPECT_EQ(1, isEqual(plt2Data.d, k2 * -1677.15625));

    plate_t plt3(pt1, pt3, pt4);
    plateData_t plt3Data = plt3.getData();
    coord_t n3Coord = plt3Data.n.getCoord();
    vector_t trueVec3(-77.07, 331.065, 243.775);
    double k3 = abs(n3Coord.x / -77.07);
    EXPECT_EQ(1, plt3Data.n.isParallel(trueVec3));
    EXPECT_EQ(1, isEqual(plt3Data.d, k3 * -571.27875));
}

TEST(plateTest, DistToPoint) {
    point_t pt1(1.5, -4, 8.25);
    point_t pt2(7, 10, -4.25);
    point_t pt3(5, 3, -0.15);
    point_t pt4(-30.1, 2.45, -10.5);

    plate_t plt1(pt1, pt2, pt3);
    double dist1 = plt1.distToPoint(pt4);
    EXPECT_EQ(1, isEqual(dist1, 36.4008170));

    plate_t plt2(pt1, pt2, pt4);
    double dist2 = plt2.distToPoint(pt3);
    EXPECT_EQ(1, isEqual(dist2, -1.6303834));

    plate_t plt3(pt1, pt3, pt4);
    double dist3 = plt3.distToPoint(pt2);
    double dist4 = plt3.distToPoint(pt1);
    EXPECT_EQ(1, isEqual(dist3, 2.78234169));
    EXPECT_EQ(1, isEqual(dist4, 0));
}

TEST(plateTest, ProjectionOfPoint) {
    point_t pt1(1.5, -4, 8.25);
    point_t pt2(7, 10, -4.25);
    point_t pt3(5, 3, -0.15);
    point_t pt4(-30.1, 2.45, -10.5);

    plate_t plt1(pt1, pt2, pt3);
    point_t projPt1 = plt1.projOfPoint(pt4);
    point_t trueProj1(4.168603, -0.339305, 1.454164);
    EXPECT_EQ(1, projPt1.isEqual(trueProj1));

    plate_t plt2(pt1, pt2, pt4);
    point_t projPt2 = plt2.projOfPoint(pt3);
    point_t trueProj2(4.584606035, 4.1376941241, 0.9414440744);
    EXPECT_EQ(1, projPt2.isEqual(trueProj2));

    plate_t plt3(pt1, pt3, pt4);
    point_t projPt3 = plt3.projOfPoint(pt2);
    point_t trueProj3(7.5126417121, 7.7978755882, -5.8715029632);
    point_t projPt4 = plt3.projOfPoint(pt4);
    EXPECT_EQ(1, projPt3.isEqual(trueProj3));
    EXPECT_EQ(1, projPt4.isEqual(pt4));
}

TEST(plateTest, ProjectionOfVector) {
    point_t pt1(1.5, -4, 8.25);
    point_t pt2(7, 10, -4.25);
    point_t pt3(5, 3, -0.15);

    plate_t plt(pt1, pt2, pt3);

    vector_t vec1(-30.1, 2.45, -10.5);
    vector_t projVec1 = plt.projOfVector(vec1);
    vector_t trueProj1(0, 0, 0);
    EXPECT_EQ(1, projVec1.isEqual(trueProj1));

    vector_t vec2(9.23, -6.437, 5.32);
    vector_t projVec2 = plt.projOfVector(vec2);
    vector_t trueProj2(-1.0594852007, -5.5994837627, 1.7306446974);
    EXPECT_EQ(1, projVec2.isEqual(trueProj2));

    vector_t vec3(-7.453, -2.345, 10.4386);
    vector_t projVec3 = plt.projOfVector(vec3);
    vector_t trueProj3(-4.2439994008, -2.6061977232, 11.5580188137);
    EXPECT_EQ(1, projVec3.isEqual(trueProj3));
}

TEST(plateTest, intsectOf2Plt) {
    point_t pt1(1.5, -4, 8.25);
    point_t pt2(7, 10, -4.25);
    point_t pt3(5, 3, -0.15);

    point_t pt4(-30.1, 2.45, -10.5);
    point_t pt5(9.23, -6.437, 5.32);
    point_t pt6(-7.453, -2.345, 10.4386);

    plate_t plt1(pt1, pt2, pt3);
    plate_t plt2(pt4, pt5, pt6);

    line_t line1 = plt1.intsectOf2Plt(plt2);
    lineData_t lineData = line1.getData();

    point_t truePt(3.6503397651, -5.210893188, 1.8031509295);
    vector_t trueDir1(4853.95825845, -1538.920425, -14273.76177339);

    line_t trueline(trueDir1, truePt);

    EXPECT_EQ(1, trueDir1.isParallel(lineData.a));
    EXPECT_EQ(1, trueline.onLine(lineData.linePt));
}


// TRIANGLE TEST
TEST(triangleTest, Intersection2D) {
    point_t pt1(0, 0, 0);
    point_t pt2(1, 0, 0);
    point_t pt3(0, 1, 0);

    point_t pt4(0, 0, 0);
    point_t pt5(2, 0, 0);
    point_t pt6(0, 2, 0);

    triangle_t triag1(pt1, pt2, pt3);
    triangle_t triag2(pt4, pt5, pt6);

    plate_t plt(pt1, pt2, pt3);

    bool res = triag1.isIntersection2D(triag2, plt);
    EXPECT_EQ(1, res);

    point_t pt7(3, 3, 3);
    triangle_t triag3(pt7, pt5, pt6);

    res = triag1.isIntersection2D(triag3, plt);
    EXPECT_EQ(0, res);
}