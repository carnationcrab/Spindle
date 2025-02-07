#define _USE_MATH_DEFINES

#include "SpindleTest.h"
#include "../Math/Sphere.h"
#include "../Math/Point.h"
#include "../Math/Vector.h"
#include <cmath>

using namespace Spindle;

TEST_CASE(Sphere_DefaultConstructor) {
    Sphere<float> sphere;
    SpindleTest::assertEqual(sphere.getCentre(), Point<float, 3>(0.0f, 0.0f, 0.0f), "Default constructor should initialize centre to origin");
    SpindleTest::assertEqual(sphere.getRadius(), 0.0f, "Default constructor should initialize radius to zero");
}

TEST_CASE(Sphere_ParameterizedConstructor) {
    Point<float, 3> centre(1.0f, 2.0f, 3.0f);
    float radius = 5.0f;
    Sphere<float> sphere(centre, radius);

    SpindleTest::assertEqual(sphere.getCentre(), centre, "Parameterized constructor should set centre correctly");
    SpindleTest::assertEqual(sphere.getRadius(), radius, "Parameterized constructor should set radius correctly");
}

TEST_CASE(Sphere_Contains) {
    Sphere<float> sphere(Point<float, 3>(0.0f, 0.0f, 0.0f), 5.0f);

    // Inside the sphere
    Point<float, 3> insidePoint(3.0f, 4.0f, 0.0f);
    SpindleTest::assertTrue(sphere.contains(insidePoint), "Sphere should contain the point (3, 4, 0)");

    // On the surface of the sphere
    Point<float, 3> surfacePoint(5.0f, 0.0f, 0.0f);
    SpindleTest::assertTrue(sphere.contains(surfacePoint), "Sphere should contain the point (5, 0, 0)");

    // Outside the sphere
    Point<float, 3> outsidePoint(6.0f, 0.0f, 0.0f);
    SpindleTest::assertFalse(sphere.contains(outsidePoint), "Sphere should not contain the point (6, 0, 0)");
}

TEST_CASE(Sphere_Intersects) {
    Sphere<float> sphere1(Point<float, 3>(0.0f, 0.0f, 0.0f), 5.0f);
    Sphere<float> sphere2(Point<float, 3>(7.0f, 0.0f, 0.0f), 3.0f);
    Sphere<float> sphere3(Point<float, 3>(10.0f, 0.0f, 0.0f), 3.0f);

    // Intersecting spheres
    SpindleTest::assertTrue(sphere1.intersects(sphere2), "Sphere1 should intersect Sphere2");

    // Non-intersecting spheres
    SpindleTest::assertFalse(sphere1.intersects(sphere3), "Sphere1 should not intersect Sphere3");
}

TEST_CASE(Sphere_Volume) {
    Sphere<float> sphere(Point<float, 3>(0.0f, 0.0f, 0.0f), 3.0f);
    float expectedVolume = (4.0f / 3.0f) * M_PI * 3.0f * 3.0f * 3.0f;
    SpindleTest::assertEqual(sphere.volume(), expectedVolume, "Volume calculation should match expected value");
}

TEST_CASE(Sphere_SurfaceArea) {
    Sphere<float> sphere(Point<float, 3>(0.0f, 0.0f, 0.0f), 3.0f);
    float expectedSurfaceArea = 4.0f * M_PI * 3.0f * 3.0f;
    SpindleTest::assertEqual(sphere.surfaceArea(), expectedSurfaceArea, "Surface area calculation should match expected value");
}

TEST_CASE(Sphere_SettersAndGetters) {
    Sphere<float> sphere;

    Point<float, 3> newCentre(1.0f, 2.0f, 3.0f);
    float newRadius = 5.0f;

    sphere.setCentre(newCentre);
    sphere.setRadius(newRadius);

    SpindleTest::assertEqual(sphere.getCentre(), newCentre, "setCentre should update the centre correctly");
    SpindleTest::assertEqual(sphere.getRadius(), newRadius, "setRadius should update the radius correctly");
}

TEST_CASE(Sphere_ToString) {
    Sphere<float> sphere(Point<float, 3>(1.0f, 2.0f, 3.0f), 5.0f);
    std::string expected = "Sphere(Centre: (1.000000, 2.000000, 3.000000), Radius: 5.000000)";
    SpindleTest::assertEqual(sphere.toString(), expected, "toString should produce the correct string representation of the sphere");
}
