#include "SpindleTest.h"
#include "../Math/Point.h"
#include "../Math/Vector.h"

using namespace Spindle;

TEST_CASE(Point2D_DefaultConstructor) {
    Point<float, 2> p;
    SpindleTest::assertEqual(p.x, 0.0f, "Default constructor X should be 0.0f");
    SpindleTest::assertEqual(p.y, 0.0f, "Default constructor Y should be 0.0f");
}

TEST_CASE(Point2D_ParameterizedConstructor) {
    Point<float, 2> p(3.0f, 4.0f);
    SpindleTest::assertEqual(p.x, 3.0f, "Parameterized constructor X should be 3.0f");
    SpindleTest::assertEqual(p.y, 4.0f, "Parameterized constructor Y should be 4.0f");
}

TEST_CASE(Point2D_AdditionWithVector) {
    Point<float, 2> p(1.0f, 2.0f);
    Vector<float, 2> v(3.0f, 4.0f);
    Point<float, 2> result = p + v;
    SpindleTest::assertEqual(result.x, 4.0f, "Addition X should be 4.0f");
    SpindleTest::assertEqual(result.y, 6.0f, "Addition Y should be 6.0f");
}

TEST_CASE(Point2D_SubtractionToVector) {
    Point<float, 2> p1(5.0f, 6.0f);
    Point<float, 2> p2(3.0f, 4.0f);
    Vector<float, 2> result = p1 - p2;
    SpindleTest::assertEqual(result.x, 2.0f, "Subtraction X should be 2.0f");
    SpindleTest::assertEqual(result.y, 2.0f, "Subtraction Y should be 2.0f");
}

TEST_CASE(Point2D_ScalarMultiplication) {
    Point<float, 2> p(2.0f, 3.0f);
    Point<float, 2> result = p * 2.0f;
    SpindleTest::assertEqual(result.x, 4.0f, "Scalar multiplication X should be 4.0f");
    SpindleTest::assertEqual(result.y, 6.0f, "Scalar multiplication Y should be 6.0f");
}

TEST_CASE(Point3D_ScalarMultiplication) {
    Point<float, 3> p(2.0f, 3.0f, 4.0f);
    Point<float, 3> result = p * 2.0f;
    SpindleTest::assertEqual(result.x, 4.0f, "Scalar multiplication X should be 4.0f");
    SpindleTest::assertEqual(result.y, 6.0f, "Scalar multiplication Y should be 6.0f");
    SpindleTest::assertEqual(result.z, 8.0f, "Scalar multiplication Z should be 8.0f");
}

TEST_CASE(Point2D_AdditionWithPoint) {
    Point<float, 2> p1(1.0f, 2.0f);
    Point<float, 2> p2(3.0f, 4.0f);
    Point<float, 2> result = p1 + p2;
    SpindleTest::assertEqual(result.x, 4.0f, "Addition X should be 4.0f");
    SpindleTest::assertEqual(result.y, 6.0f, "Addition Y should be 6.0f");
}

TEST_CASE(Point3D_AdditionWithPoint) {
    Point<float, 3> p1(1.0f, 2.0f, 3.0f);
    Point<float, 3> p2(4.0f, 5.0f, 6.0f);
    Point<float, 3> result = p1 + p2;
    SpindleTest::assertEqual(result.x, 5.0f, "Addition X should be 5.0f");
    SpindleTest::assertEqual(result.y, 7.0f, "Addition Y should be 7.0f");
    SpindleTest::assertEqual(result.z, 9.0f, "Addition Z should be 9.0f");
}

TEST_CASE(Point2D_Lerp) {
    Point<float, 2> p1(0.0f, 0.0f);
    Point<float, 2> p2(2.0f, 2.0f);
    Point<float, 2> result = p1.lerp(p2, 0.5f);
    SpindleTest::assertEqual(result.x, 1.0f, "Lerp X should be 1.0f");
    SpindleTest::assertEqual(result.y, 1.0f, "Lerp Y should be 1.0f");
}

TEST_CASE(Point3D_Lerp) {
    Point<float, 3> p1(0.0f, 0.0f, 0.0f);
    Point<float, 3> p2(2.0f, 2.0f, 2.0f);
    Point<float, 3> result = p1.lerp(p2, 0.5f);
    SpindleTest::assertEqual(result.x, 1.0f, "Lerp X should be 1.0f");
    SpindleTest::assertEqual(result.y, 1.0f, "Lerp Y should be 1.0f");
    SpindleTest::assertEqual(result.z, 1.0f, "Lerp Z should be 1.0f");
}

TEST_CASE(Point2D_DistanceBetween) {
    Point<float, 2> p1(0.0f, 0.0f);
    Point<float, 2> p2(3.0f, 4.0f);
    float result = p1.distanceTo(p2);
    SpindleTest::assertEqual(result, 5.0f, "Distance should be 5.0f");
}

TEST_CASE(Point2D_DistanceSquaredBetween) {
    Point<float, 2> p1(0.0f, 0.0f);
    Point<float, 2> p2(3.0f, 4.0f);
    float result = p1.distanceSquaredTo(p2);
    SpindleTest::assertEqual(result, 25.0f, "Squared distance should be 25.0f");
}

TEST_CASE(Point2D_EqualityOperator) {
    Point<float, 2> p1(1.0f, 2.0f);
    Point<float, 2> p2(1.0f, 2.0f);
    Point<float, 2> p3(2.0f, 3.0f);
    SpindleTest::assertTrue(p1 == p2, "Points should be equal");
    SpindleTest::assertTrue(p1 != p3, "Points should not be equal");
}

TEST_CASE(Point2D_ToString) {
    Point<float, 2> p(1.0f, 2.0f);
    SpindleTest::assertTrue(p.toString() == "(1.000000, 2.000000)", "toString output should be (1.000000, 2.000000)");
}

TEST_CASE(Point3D_DefaultConstructor) {
    Point<float, 3> p;
    SpindleTest::assertEqual(p.x, 0.0f, "Default constructor X should be 0.0f");
    SpindleTest::assertEqual(p.y, 0.0f, "Default constructor Y should be 0.0f");
    SpindleTest::assertEqual(p.z, 0.0f, "Default constructor Z should be 0.0f");
}

TEST_CASE(Point3D_ParameterizedConstructor) {
    Point<float, 3> p(3.0f, 4.0f, 5.0f);
    SpindleTest::assertEqual(p.x, 3.0f, "Parameterized constructor X should be 3.0f");
    SpindleTest::assertEqual(p.y, 4.0f, "Parameterized constructor Y should be 4.0f");
    SpindleTest::assertEqual(p.z, 5.0f, "Parameterized constructor Z should be 5.0f");
}

TEST_CASE(Point3D_AdditionWithVector) {
    Point<float, 3> p(1.0f, 2.0f, 3.0f);
    Vector<float, 3> v(3.0f, 4.0f, 5.0f);
    Point<float, 3> result = p + v;
    SpindleTest::assertEqual(result.x, 4.0f, "Addition X should be 4.0f");
    SpindleTest::assertEqual(result.y, 6.0f, "Addition Y should be 6.0f");
    SpindleTest::assertEqual(result.z, 8.0f, "Addition Z should be 8.0f");
}

TEST_CASE(Point3D_SubtractionToVector) {
    Point<float, 3> p1(5.0f, 6.0f, 7.0f);
    Point<float, 3> p2(3.0f, 4.0f, 5.0f);
    Vector<float, 3> result = p1 - p2;
    SpindleTest::assertEqual(result.x, 2.0f, "Subtraction X should be 2.0f");
    SpindleTest::assertEqual(result.y, 2.0f, "Subtraction Y should be 2.0f");
    SpindleTest::assertEqual(result.z, 2.0f, "Subtraction Z should be 2.0f");
}

TEST_CASE(Point3D_DistanceBetween) {
    Point<float, 3> p1(0.0f, 0.0f, 0.0f);
    Point<float, 3> p2(3.0f, 4.0f, 5.0f);
    float result = p1.distanceTo(p2);
    SpindleTest::assertEqual(result, std::sqrt(50.0f), "Distance should be sqrt(50.0f)");
}

TEST_CASE(Point3D_DistanceSquaredBetween) {
    Point<float, 3> p1(0.0f, 0.0f, 0.0f);
    Point<float, 3> p2(3.0f, 4.0f, 5.0f);
    float result = p1.distanceSquaredTo(p2);
    SpindleTest::assertEqual(result, 50.0f, "Squared distance should be 50.0f");
}

TEST_CASE(Point3D_EqualityOperator) {
    Point<float, 3> p1(1.0f, 2.0f, 3.0f);
    Point<float, 3> p2(1.0f, 2.0f, 3.0f);
    Point<float, 3> p3(2.0f, 3.0f, 4.0f);
    SpindleTest::assertTrue(p1 == p2, "Points should be equal");
    SpindleTest::assertTrue(p1 != p3, "Points should not be equal");
}

TEST_CASE(Point3D_ToString) {
    Point<float, 3> p(1.0f, 2.0f, 3.0f);
    SpindleTest::assertTrue(p.toString() == "(1.000000, 2.000000, 3.000000)", "toString output should be (1.000000, 2.000000, 3.000000)");
}
