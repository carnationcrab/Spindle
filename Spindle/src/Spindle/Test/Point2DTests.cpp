#include "SpindleTest.h"
#include "../Math/Point2D.h"
#include "../Math/Vector2.h"

using namespace Spindle;

TEST_CASE(Point2D_DefaultConstructor) {
    Point2D p;
    return SpindleTest::assertEqual(p.x, 0.0f, "Default constructor X should be 0.0f")
        && SpindleTest::assertEqual(p.y, 0.0f, "Default constructor Y should be 0.0f");
}

TEST_CASE(Point2D_ParameterizedConstructor) {
    Point2D p(3.0f, 4.0f);
    return SpindleTest::assertEqual(p.x, 3.0f, "Parameterized constructor X should be 3.0f")
        && SpindleTest::assertEqual(p.y, 4.0f, "Parameterized constructor Y should be 4.0f");
}

TEST_CASE(Point2D_AdditionWithVector) {
    Point2D p(1.0f, 2.0f);
    Vector2<float> v(3.0f, 4.0f);
    Point2D result = p + v;
    return SpindleTest::assertEqual(result.x, 4.0f, "Addition X should be 4.0f")
        && SpindleTest::assertEqual(result.y, 6.0f, "Addition Y should be 6.0f");
}

TEST_CASE(Point2D_SubtractionToVector) {
    Point2D p1(5.0f, 6.0f);
    Point2D p2(3.0f, 4.0f);
    Vector2<float> result = p1 - p2;
    return SpindleTest::assertEqual(result.getX(), 2.0f, "Subtraction X should be 2.0f")
        && SpindleTest::assertEqual(result.getY(), 2.0f, "Subtraction Y should be 2.0f");
}

TEST_CASE(Point2D_DistanceBetween) {
    Point2D p1(0.0f, 0.0f);
    Point2D p2(3.0f, 4.0f);
    float result = p1.distanceBetween(p2);
    return SpindleTest::assertEqual(result, 5.0f, "Distance should be 5.0f");
}

TEST_CASE(Point2D_DistanceSquaredBetween) {
    Point2D p1(0.0f, 0.0f);
    Point2D p2(3.0f, 4.0f);
    float result = p1.distanceSquaredBetween(p2);
    return SpindleTest::assertEqual(result, 25.0f, "Squared distance should be 25.0f");
}

TEST_CASE(Point2D_EqualityOperator) {
    Point2D p1(1.0f, 2.0f);
    Point2D p2(1.0f, 2.0f);
    Point2D p3(2.0f, 3.0f);
    return SpindleTest::assertTrue(p1 == p2, "Points should be equal")
        && SpindleTest::assertTrue(p1 != p3, "Points should not be equal");
}

TEST_CASE(Point2D_ToString) {
    Point2D p(1.0f, 2.0f);
    return SpindleTest::assertTrue(p.toString() == "(1.000000, 2.000000)", "toString output should be (1.000000, 2.000000)");
}
