#include "SpindleTest.h"
#include "../Math/Ray.h"
#include "../Math/Point.h"
#include "../Math/Vector.h"

using namespace Spindle;

TEST_CASE(Ray_DefaultConstructor) {
    Ray<float, 3> ray;
    SpindleTest::assertEqual(ray.line.point, Point<float, 3>(), "Default constructor should initialize origin to zero point");
    SpindleTest::assertEqual(ray.line.direction, Vector<float, 3>(), "Default constructor should initialize direction to zero vector");
}

TEST_CASE(Ray_ParameterizedConstructor) {
    Vector<float, 3> direction(0.0f, 1.0f, 0.0f);
    Point<float, 3> origin(1.0f, 2.0f, 3.0f);
    Ray<float, 3> ray(origin, direction);

    SpindleTest::assertEqual(ray.line.point, origin, "Parameterized constructor should set origin correctly");
    SpindleTest::assertEqual(ray.line.direction, direction.unitVector(), "Parameterized constructor should normalize direction");
}

TEST_CASE(Ray_GetPoint) {
    Vector<float, 3> direction(1.0f, 0.0f, 0.0f);
    Point<float, 3> origin(1.0f, 2.0f, 3.0f);
    Ray<float, 3> ray(origin, direction);

    Point<float, 3> result = ray.getPoint(5.0f);
    SpindleTest::assertEqual(result, Point<float, 3>(6.0f, 2.0f, 3.0f), "getPoint should calculate the correct point on the ray");

    // Test t = 0
    result = ray.getPoint(0.0f);
    SpindleTest::assertEqual(result, origin, "getPoint with t=0 should return the origin");
}

TEST_CASE(Ray_EqualityOperators) {
    Vector<float, 3> direction1(1.0f, 0.0f, 0.0f);
    Point<float, 3> origin1(1.0f, 2.0f, 3.0f);
    Ray<float, 3> ray1(origin1, direction1);

    Vector<float, 3> direction2(1.0f, 0.0f, 0.0f);
    Point<float, 3> origin2(1.0f, 2.0f, 3.0f);
    Ray<float, 3> ray2(origin2, direction2);

    Vector<float, 3> direction3(0.0f, 1.0f, 0.0f);
    Point<float, 3> origin3(0.0f, 0.0f, 0.0f);
    Ray<float, 3> ray3(origin3, direction3);

    SpindleTest::assertTrue(ray1 == ray2, "Equality operator should return true for identical rays");
    SpindleTest::assertTrue(ray1 != ray3, "Inequality operator should return true for different rays");
}

TEST_CASE(Ray_ToString) {
    Vector<float, 3> direction(1.0f, 0.0f, 0.0f);
    Point<float, 3> origin(1.0f, 2.0f, 3.0f);
    Ray<float, 3> ray(origin, direction);

    std::string expected = "Ray(Line(point: (1.000000, 2.000000, 3.000000), Direction: (1.000000, 0.000000, 0.000000)))";
    SpindleTest::assertEqual(ray.toString(), expected, "toString should produce the correct string representation of the ray");
}
