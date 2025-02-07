#include "SpindleTest.h"
#include "../Math/Plane.h"
#include "../Math/Point.h"
#include "../Math/Vector.h"

using namespace Spindle;

TEST_CASE(Plane_DefaultConstructor) {
    Plane<float> plane;
    SpindleTest::assertEqual(plane.getNormal(), Vector<float, 3>(0.0f, 0.0f, 0.0f), "Default constructor should initialize normal to zero vector");
    SpindleTest::assertEqual(plane.getDistance(), 0.0f, "Default constructor should initialize distance to zero");
}

TEST_CASE(Plane_NormalDistanceConstructor) {
    Vector<float, 3> normal(1.0f, 0.0f, 0.0f);
    float distance = 5.0f;
    Plane<float> plane(normal, distance);

    SpindleTest::assertEqual(plane.getNormal(), normal.unitVector(), "Constructor should normalize the normal vector");
    SpindleTest::assertEqual(plane.getDistance(), distance, "Constructor should set the distance correctly");
}

TEST_CASE(Plane_PointNormalConstructor) {
    Point<float, 3> point(1.0f, 2.0f, 3.0f);
    Vector<float, 3> normal(0.0f, 1.0f, 0.0f);
    Plane<float> plane(point, normal);

    SpindleTest::assertEqual(plane.getNormal(), normal.unitVector(), "Constructor should normalize the normal vector");
    // SpindleTest::assertEqual(plane.getDistance(), -(normal.unitVector().dot(point)), "Constructor should calculate distance correctly");
}

TEST_CASE(Plane_SignedDistance) {
    Vector<float, 3> normal(0.0f, 1.0f, 0.0f);
    float distance = -5.0f;
    Plane<float> plane(normal, distance);

    Point<float, 3> pointOnPlane(0.0f, 5.0f, 0.0f);
    Point<float, 3> pointAbovePlane(0.0f, 10.0f, 0.0f);
    Point<float, 3> pointBelowPlane(0.0f, 0.0f, 0.0f);

    SpindleTest::assertEqual(plane.signedDistance(pointOnPlane), 0.0f, "Signed distance should be zero for a point on the plane");
    SpindleTest::assertEqual(plane.signedDistance(pointAbovePlane), 5.0f, "Signed distance should be positive for a point above the plane");
    SpindleTest::assertEqual(plane.signedDistance(pointBelowPlane), -5.0f, "Signed distance should be negative for a point below the plane");
}

TEST_CASE(Plane_Contains) {
    Vector<float, 3> normal(0.0f, 1.0f, 0.0f);
    float distance = -5.0f;
    Plane<float> plane(normal, distance);

    Point<float, 3> pointOnPlane(0.0f, 5.0f, 0.0f);
    Point<float, 3> pointOffPlane(0.0f, 6.0f, 0.0f);

    SpindleTest::assertTrue(plane.contains(pointOnPlane), "Contains should return true for a point on the plane");
    SpindleTest::assertFalse(plane.contains(pointOffPlane), "Contains should return false for a point off the plane");
}

TEST_CASE(Plane_SettersAndGetters) {
    Plane<float> plane;

    Vector<float, 3> newNormal(1.0f, 0.0f, 0.0f);
    float newDistance = 10.0f;

    plane.setNormal(newNormal);
    plane.setDistance(newDistance);

    SpindleTest::assertEqual(plane.getNormal(), newNormal.unitVector(), "setNormal should update the normal correctly");
    SpindleTest::assertEqual(plane.getDistance(), newDistance, "setDistance should update the distance correctly");
}

TEST_CASE(Plane_ToString) {
    Vector<float, 3> normal(0.0f, 1.0f, 0.0f);
    float distance = -5.0f;
    Plane<float> plane(normal, distance);

    std::string expected = "Plane(Normal: (0.000000, 1.000000, 0.000000), Distance: -5.000000)";
    SpindleTest::assertEqual(plane.toString(), expected, "toString should produce the correct string representation of the plane");
}
