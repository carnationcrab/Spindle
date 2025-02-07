#include "SpindleTest.h"
#include "../Math/AABB.h"
#include "../Math/Point.h"
#include "../Math/Vector.h"

using namespace Spindle;

TEST_CASE(AABB_DefaultConstructor) {
    AABB<float> aabb;
    SpindleTest::assertEqual(aabb.getMin(), Point<float, 3>(0.0f, 0.0f, 0.0f), "Default constructor should initialize min to origin");
    SpindleTest::assertEqual(aabb.getMax(), Point<float, 3>(0.0f, 0.0f, 0.0f), "Default constructor should initialize max to origin");
}

TEST_CASE(AABB_ParameterizedConstructor) {
    Point<float, 3> minPoint(1.0f, 2.0f, 3.0f);
    Point<float, 3> maxPoint(4.0f, 5.0f, 6.0f);
    AABB<float> aabb(minPoint, maxPoint);

    SpindleTest::assertEqual(aabb.getMin(), minPoint, "Parameterized constructor should set min correctly");
    SpindleTest::assertEqual(aabb.getMax(), maxPoint, "Parameterized constructor should set max correctly");
}

TEST_CASE(AABB_ContainsPoint) {
    AABB<float> aabb(Point<float, 3>(1.0f, 1.0f, 1.0f), Point<float, 3>(5.0f, 5.0f, 5.0f));

    SpindleTest::assertTrue(aabb.contains(Point<float, 3>(3.0f, 3.0f, 3.0f)), "AABB should contain point inside it");
    SpindleTest::assertTrue(aabb.contains(Point<float, 3>(1.0f, 1.0f, 1.0f)), "AABB should contain point on its min boundary");
    SpindleTest::assertTrue(aabb.contains(Point<float, 3>(5.0f, 5.0f, 5.0f)), "AABB should contain point on its max boundary");
    SpindleTest::assertFalse(aabb.contains(Point<float, 3>(6.0f, 3.0f, 3.0f)), "AABB should not contain point outside it");
}

TEST_CASE(AABB_ContainsBoundaryCases) {
    AABB<float> aabb(Point<float, 3>(1.0f, 1.0f, 1.0f), Point<float, 3>(5.0f, 5.0f, 5.0f));

    // Boundary points
    SpindleTest::assertTrue(aabb.contains(Point<float, 3>(1.0f, 3.0f, 3.0f)), "Point on the min edge should be inside the AABB");
    SpindleTest::assertTrue(aabb.contains(Point<float, 3>(5.0f, 3.0f, 3.0f)), "Point on the max edge should be inside the AABB");

    // Just outside the boundary
    SpindleTest::assertFalse(aabb.contains(Point<float, 3>(0.999f, 3.0f, 3.0f)), "Point just outside the min edge should not be inside the AABB");
    SpindleTest::assertFalse(aabb.contains(Point<float, 3>(5.001f, 3.0f, 3.0f)), "Point just outside the max edge should not be inside the AABB");
}

TEST_CASE(AABB_Intersects) {
    AABB<float> aabb1(Point<float, 3>(1.0f, 1.0f, 1.0f), Point<float, 3>(5.0f, 5.0f, 5.0f));
    AABB<float> aabb2(Point<float, 3>(4.0f, 4.0f, 4.0f), Point<float, 3>(6.0f, 6.0f, 6.0f));
    AABB<float> aabb3(Point<float, 3>(6.0f, 6.0f, 6.0f), Point<float, 3>(7.0f, 7.0f, 7.0f));

    SpindleTest::assertTrue(aabb1.intersects(aabb2), "AABB1 should intersect AABB2");
    SpindleTest::assertFalse(aabb1.intersects(aabb3), "AABB1 should not intersect AABB3");
}

TEST_CASE(AABB_IntersectsPartialOverlap) {
    // Overlap on X-axis only
    AABB<float> aabb1(Point<float, 3>(1.0f, 1.0f, 1.0f), Point<float, 3>(5.0f, 5.0f, 5.0f));
    AABB<float> aabb2(Point<float, 3>(4.0f, 6.0f, 6.0f), Point<float, 3>(6.0f, 7.0f, 7.0f));

    SpindleTest::assertFalse(aabb1.intersects(aabb2), "AABBs that only overlap along one axis should not intersect");

    // Overlap on all axes
    AABB<float> aabb3(Point<float, 3>(4.0f, 4.0f, 4.0f), Point<float, 3>(6.0f, 6.0f, 6.0f));
    SpindleTest::assertTrue(aabb1.intersects(aabb3), "AABBs that overlap on all axes should intersect");
}

TEST_CASE(AABB_ZeroSize) {
    AABB<float> aabb(Point<float, 3>(2.0f, 2.0f, 2.0f), Point<float, 3>(2.0f, 2.0f, 2.0f));

    // Should only contain the single point
    SpindleTest::assertTrue(aabb.contains(Point<float, 3>(2.0f, 2.0f, 2.0f)), "Zero-size AABB should contain its single point");
    SpindleTest::assertFalse(aabb.contains(Point<float, 3>(3.0f, 3.0f, 3.0f)), "Zero-size AABB should not contain any other point");

    // Should not intersect with other AABBs
    AABB<float> other(Point<float, 3>(3.0f, 3.0f, 3.0f), Point<float, 3>(4.0f, 4.0f, 4.0f));
    SpindleTest::assertFalse(aabb.intersects(other), "Zero-size AABB should not intersect with another AABB");
}


TEST_CASE(AABB_Center) {
    AABB<float> aabb(Point<float, 3>(1.0f, 1.0f, 1.0f), Point<float, 3>(5.0f, 5.0f, 5.0f));
    Point<float, 3> expectedCenter(3.0f, 3.0f, 3.0f);

    SpindleTest::assertEqual(aabb.center(), expectedCenter, "Center calculation should be correct");
}

TEST_CASE(AABB_Volume) {
    AABB<float> aabb(Point<float, 3>(1.0f, 1.0f, 1.0f), Point<float, 3>(5.0f, 5.0f, 5.0f));
    float expectedVolume = 4.0f * 4.0f * 4.0f;

    SpindleTest::assertEqual(aabb.volume(), expectedVolume, "Volume calculation should be correct");
}

TEST_CASE(AABB_ExpandToIncludePoint) {
    AABB<float> aabb(Point<float, 3>(1.0f, 1.0f, 1.0f), Point<float, 3>(3.0f, 3.0f, 3.0f));
    Point<float, 3> point(5.0f, 5.0f, 5.0f);

    aabb.expandToInclude(point);

    SpindleTest::assertEqual(aabb.getMin(), Point<float, 3>(1.0f, 1.0f, 1.0f), "Min should remain unchanged");
    SpindleTest::assertEqual(aabb.getMax(), Point<float, 3>(5.0f, 5.0f, 5.0f), "Max should expand to include the point");
}

TEST_CASE(AABB_ExpandToIncludeAABB) {
    AABB<float> aabb1(Point<float, 3>(1.0f, 1.0f, 1.0f), Point<float, 3>(3.0f, 3.0f, 3.0f));
    AABB<float> aabb2(Point<float, 3>(2.0f, 2.0f, 2.0f), Point<float, 3>(5.0f, 5.0f, 5.0f));

    aabb1.expandToInclude(aabb2);

    SpindleTest::assertEqual(aabb1.getMin(), Point<float, 3>(1.0f, 1.0f, 1.0f), "Min should remain unchanged");
    SpindleTest::assertEqual(aabb1.getMax(), Point<float, 3>(5.0f, 5.0f, 5.0f), "Max should expand to include the other AABB");
}

TEST_CASE(AABB_ToString) {
    AABB<float> aabb(Point<float, 3>(1.0f, 2.0f, 3.0f), Point<float, 3>(4.0f, 5.0f, 6.0f));
    std::string expected = "AABB(Min: (1, 2, 3, 0, 0, 0, 0, 0), Max: (4, 5, 6, 0, 0, 0, 0, 0))";

    SpindleTest::assertEqual(aabb.toString(), expected, "toString should produce the correct string representation");
}

//TEST_CASE(AABB_InvalidInput) {
//    AABB<float> invalidAABB(Point<float, 3>(5.0f, 5.0f, 5.0f), Point<float, 3>(1.0f, 1.0f, 1.0f));
//
//    SpindleTest::assertFalse(invalidAABB.isValid(), "Invalid AABB should be flagged as invalid");
//    SpindleTest::assertFalse(invalidAABB.contains(Point<float, 3>(3.0f, 3.0f, 3.0f)), "Invalid AABB should not contain any points");
//    AABB<float> validAABB(Point<float, 3>(0.0f, 0.0f, 0.0f), Point<float, 3>(6.0f, 6.0f, 6.0f));
//    SpindleTest::assertFalse(invalidAABB.intersects(validAABB), "Invalid AABB should not intersect any valid AABB");
//}
//
//TEST_CASE(AABB_HandleNaN) {
//    AABB<float> invalidAABB(Point<float, 3>(std::nanf(""), 1.0f, 1.0f), Point<float, 3>(1.0f, 1.0f, 1.0f));
//    SpindleTest::assertFalse(invalidAABB.isValid(), "AABB with NaN in min should be invalid");
//
//    invalidAABB = AABB<float>(Point<float, 3>(1.0f, 1.0f, 1.0f), Point<float, 3>(std::nanf(""), 1.0f, 1.0f));
//    SpindleTest::assertFalse(invalidAABB.isValid(), "AABB with NaN in max should be invalid");
//}


