#include "../Math/Line.h"
#include "../Math/Point.h"
#include "../Math/Vector.h"
#include "SpindleTest.h"

using namespace Spindle;

TEST_CASE(Line_DefaultConstructor) {
    Line<float, 3> line;
    SpindleTest::assertEqual(line.start, Point<float, 3>(), "Default constructor should initialize start to the origin");
    SpindleTest::assertEqual(line.direction, Vector<float, 3>(), "Default constructor should initialize direction to zero vector");
}

TEST_CASE(Line_ParameterizedConstructor) {
    Point<float, 3> start(1.0f, 2.0f, 3.0f);
    Vector<float, 3> dir(0.0f, 1.0f, 0.0f);
    Line<float, 3> line(start, dir);

    SpindleTest::assertEqual(line.start, start, "Parameterized constructor should set start correctly");
    SpindleTest::assertEqual(line.direction, dir.unitVector(), "Parameterized constructor should normalize the direction vector");
}

TEST_CASE(Line_GetPoint) {
    Point<float, 3> start(1.0f, 2.0f, 3.0f);
    Vector<float, 3> dir(1.0f, 0.0f, 0.0f);
    Line<float, 3> line(start, dir);

    Point<float, 3> point = line.getPoint(5.0f);
    SpindleTest::assertEqual(point, Point<float, 3>(6.0f, 2.0f, 3.0f), "getPoint should calculate the correct point on the line");
}

TEST_CASE(Line_Equality) {
    Point<float, 3> start(1.0f, 2.0f, 3.0f);
    Vector<float, 3> dir(0.0f, 1.0f, 0.0f);
    Line<float, 3> line1(start, dir);
    Line<float, 3> line2(start, dir);

    SpindleTest::assertTrue(line1 == line2, "Equality operator should return true for identical lines");
}

TEST_CASE(Line_Inequality) {
    Point<float, 3> start1(1.0f, 2.0f, 3.0f);
    Vector<float, 3> dir1(0.0f, 1.0f, 0.0f);
    Line<float, 3> line1(start1, dir1);

    Point<float, 3> start2(0.0f, 0.0f, 0.0f);
    Vector<float, 3> dir2(1.0f, 0.0f, 0.0f);
    Line<float, 3> line2(start2, dir2);

    SpindleTest::assertTrue(line1 != line2, "Inequality operator should return true for different lines");
}

TEST_CASE(Line_ToString) {
    Point<float, 3> start(1.0f, 2.0f, 3.0f);
    Vector<float, 3> dir(0.0f, 1.0f, 0.0f);
    Line<float, 3> line(start, dir);

    std::string expected = "Line(start: (1.000000, 2.000000, 3.000000), Direction: (0.000000, 1.000000, 0.000000))";
    SpindleTest::assertEqual(line.ToString(), expected, "toString should return the correct representation of the line");
}
