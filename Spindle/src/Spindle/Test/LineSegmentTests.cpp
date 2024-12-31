#include "SpindleTest.h"
#include "../Math/LineSegment.h"
#include "../Math/Point.h"
#include "../Math/Vector.h"

using namespace Spindle;

TEST_CASE(LineSegment_DefaultConstructor) {
    LineSegment<float, 3> segment;
    SpindleTest::assertEqual(segment.start, Point<float, 3>(), "Default constructor should initialize start to zero point");
    SpindleTest::assertEqual(segment.end, Point<float, 3>(), "Default constructor should initialize end to zero point");
}

TEST_CASE(LineSegment_ParameterizedConstructor) {
    Point<float, 3> start(1.0f, 2.0f, 3.0f);
    Point<float, 3> end(4.0f, 5.0f, 6.0f);
    LineSegment<float, 3> segment(start, end);

    SpindleTest::assertEqual(segment.start, start, "Parameterized constructor should set start point correctly");
    SpindleTest::assertEqual(segment.end, end, "Parameterized constructor should set end point correctly");
}

TEST_CASE(LineSegment_GetPoint_Normalized) {
    Point<float, 3> start(0.0f, 0.0f, 0.0f);
    Point<float, 3> end(1.0f, 1.0f, 1.0f);
    LineSegment<float, 3> segment(start, end);

    Point<float, 3> midpoint = segment.getPoint(0.5f);
    SpindleTest::assertEqual(midpoint, Point<float, 3>(0.5f, 0.5f, 0.5f), "getPoint with t=0.5 should return the midpoint");

    Point<float, 3> startPoint = segment.getPoint(0.0f);
    SpindleTest::assertEqual(startPoint, start, "getPoint with t=0 should return the start point");

    Point<float, 3> endPoint = segment.getPoint(1.0f);
    SpindleTest::assertEqual(endPoint, end, "getPoint with t=1 should return the end point");
}

TEST_CASE(LineSegment_GetPoint_NonNormalized) {
    Point<float, 3> start(0.0f, 0.0f, 0.0f);
    Point<float, 3> end(3.0f, 4.0f, 0.0f);
    LineSegment<float, 3> segment(start, end);

    Point<float, 3> midpoint = segment.getPointNonNormalized(2.5f);
    SpindleTest::assertEqual(midpoint, Point<float, 3>(1.5f, 2.0f, 0.0f), "getPointNonNormalized with t=2.5 should return the correct point");

    Point<float, 3> startPoint = segment.getPointNonNormalized(0.0f);
    SpindleTest::assertEqual(startPoint, start, "getPointNonNormalized with t=0 should return the start point");

    Point<float, 3> endPoint = segment.getPointNonNormalized(5.0f);
    SpindleTest::assertEqual(endPoint, end, "getPointNonNormalized with t=5 should return the end point");
}

TEST_CASE(LineSegment_Length) {
    Point<float, 3> start(0.0f, 0.0f, 0.0f);
    Point<float, 3> end(3.0f, 4.0f, 0.0f);
    LineSegment<float, 3> segment(start, end);

    float length = segment.length();
    SpindleTest::assertEqual(length, 5.0f, "length should calculate the correct length of the segment");
}

TEST_CASE(LineSegment_LengthSquared) {
    Point<float, 3> start(0.0f, 0.0f, 0.0f);
    Point<float, 3> end(3.0f, 4.0f, 0.0f);
    LineSegment<float, 3> segment(start, end);

    float lengthSquared = segment.lengthSquared();
    SpindleTest::assertEqual(lengthSquared, 25.0f, "lengthSquared should calculate the correct squared length of the segment");
}

TEST_CASE(LineSegment_EqualityOperators) {
    Point<float, 3> start1(0.0f, 0.0f, 0.0f);
    Point<float, 3> end1(1.0f, 1.0f, 1.0f);
    LineSegment<float, 3> segment1(start1, end1);

    Point<float, 3> start2(0.0f, 0.0f, 0.0f);
    Point<float, 3> end2(1.0f, 1.0f, 1.0f);
    LineSegment<float, 3> segment2(start2, end2);

    Point<float, 3> start3(1.0f, 1.0f, 1.0f);
    Point<float, 3> end3(2.0f, 2.0f, 2.0f);
    LineSegment<float, 3> segment3(start3, end3);

    SpindleTest::assertTrue(segment1 == segment2, "Equality operator should return true for identical segments");
    SpindleTest::assertTrue(segment1 != segment3, "Inequality operator should return true for different segments");
}

TEST_CASE(LineSegment_ToString) {
    Point<float, 3> start(1.0f, 2.0f, 3.0f);
    Point<float, 3> end(4.0f, 5.0f, 6.0f);
    LineSegment<float, 3> segment(start, end);

    std::string expected = "LineSegment(Start: (1.000000, 2.000000, 3.000000), End: (4.000000, 5.000000, 6.000000))";
    SpindleTest::assertEqual(segment.toString(), expected, "toString should produce the correct string representation of the line segment");
}
