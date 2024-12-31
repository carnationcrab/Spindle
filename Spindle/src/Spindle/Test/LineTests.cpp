#include "SpindleTest.h"
#include "../Math/Line.h"
#include "../Math/Point.h"
#include "../Math/Vector.h"

using namespace Spindle;

TEST_CASE(Line_DefaultConstructor) {
    Line<float, 3> line;
    SpindleTest::assertEqual(line.point, Point<float, 3>(), "Default constructor should initialize point to zero point");
    SpindleTest::assertEqual(line.direction, Vector<float, 3>(), "Default constructor should initialize direction to zero vector");
}

TEST_CASE(Line_ParameterizedConstructor) {
    Point<float, 3> point(1.0f, 2.0f, 3.0f);
    Vector<float, 3> direction(0.0f, 1.0f, 0.0f);
    Line<float, 3> line(point, direction);

    SpindleTest::assertEqual(line.point, point, "Parameterized constructor should set point correctly");
    SpindleTest::assertEqual(line.direction, direction.unitVector(), "Parameterized constructor should normalize direction");
}

TEST_CASE(Line_GetPoint) {
    Point<float, 3> point(1.0f, 2.0f, 3.0f);
    Vector<float, 3> direction(1.0f, 0.0f, 0.0f);
    Line<float, 3> line(point, direction);

    Point<float, 3> result = line.getPoint(5.0f);
    SpindleTest::assertEqual(result, Point<float, 3>(6.0f, 2.0f, 3.0f), "getPoint should calculate the correct point on the line");

    // Test t = 0
    result = line.getPoint(0.0f);
    SpindleTest::assertEqual(result, point, "getPoint with t=0 should return the reference point");

    // Test t = -1
    result = line.getPoint(-1.0f);
    SpindleTest::assertEqual(result, Point<float, 3>(0.0f, 2.0f, 3.0f), "getPoint with t=-1 should return the correct point in the negative direction");
}

TEST_CASE(Line_EqualityOperators) {
    Point<float, 3> point1(1.0f, 2.0f, 3.0f);
    Vector<float, 3> direction1(1.0f, 0.0f, 0.0f);
    Line<float, 3> line1(point1, direction1);

    Point<float, 3> point2(1.0f, 2.0f, 3.0f);
    Vector<float, 3> direction2(1.0f, 0.0f, 0.0f);
    Line<float, 3> line2(point2, direction2);

    Point<float, 3> point3(0.0f, 0.0f, 0.0f);
    Vector<float, 3> direction3(0.0f, 1.0f, 0.0f);
    Line<float, 3> line3(point3, direction3);

    SpindleTest::assertTrue(line1 == line2, "Equality operator should return true for identical lines");
    SpindleTest::assertTrue(line1 != line3, "Inequality operator should return true for different lines");
}

TEST_CASE(Line_ToString) {
    Point<float, 3> point(1.0f, 2.0f, 3.0f);
    Vector<float, 3> direction(1.0f, 0.0f, 0.0f);
    Line<float, 3> line(point, direction);

    std::string expected = "Line(point: (1.000000, 2.000000, 3.000000), Direction: (1.000000, 0.000000, 0.000000))";
    SpindleTest::assertEqual(line.ToString(), expected, "ToString should produce the correct string representation of the line");
}
