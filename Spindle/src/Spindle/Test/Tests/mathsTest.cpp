#include "mathsTest.h"
#include "../../Log.h"

namespace Spindle {

    // Scalar Maths
    void mathsTest::testBaseMaths() {
        SPINDLE_TEST_INFO("[mathsTest] Starting base math tests.");

        // SQRT(x,y);
        double actual = SQRT(4.0);
        runTest(2.0, actual, "SQRT(4.0)");

        actual = SQRT(9.0);
        runTest(3.0, actual, "SQRT(9.0)");

        // POWR(base, exponent);
        actual = POWR(2.0, 3.0);
        runTest(8.0, actual, "POWR(2.0, 3.0)");

        actual = POWR(3.0, 2.0);
        runTest(9.0, actual, "POWR(3.0, 2.0)");

        // SIN();
        actual = SIN(0.0);
        runTest(0.0, actual, "SIN(0.0)");

        actual = SIN(3.14159 / 2);
        runTest(1.0, actual, "SIN(3.14159 / 2)");

        // COS() tests
        actual = COS(0.0);
        runTest(1.0, actual, "COS(0.0)");

        actual = COS(3.14159 / 2);
        runTest(0.0, actual, "COS(3.14159 / 2)");

        actual = COS(3.14159);
        runTest(-1.0, actual, "COS(3.14159)");

        SPINDLE_TEST_INFO("[mathsTest] Finishing base math tests.");
    }

    // Vector2(x,y)
    void mathsTest::testVector2() {
        SPINDLE_TEST_INFO("[mathsTest] Starting Vector2 tests.");
        Vector2<float> vec2_a(3.0f, 4.0f);
        Vector2<float> vec2_b(1.0f, 2.0f);

        // Dot product tests
        float dotProductVector2 = vec2_a.dot(vec2_b);
        runTest(11.0f, dotProductVector2, "Dot Product of vec2_a and vec2_b");

        // Additional dot product tests
        Vector2<float> vec2_c(5.0f, 6.0f);
        float dotProductTest1 = vec2_a.dot(vec2_c);
        runTest(39.0f, dotProductTest1, "Dot Product of vec2_a and vec2_c");

        Vector2<float> vec2_d(-3.0f, -4.0f);
        float dotProductTest2 = vec2_a.dot(vec2_d);
        runTest(-25.0f, dotProductTest2, "Dot Product of vec2_a and vec2_d");

        SPINDLE_TEST_INFO("[mathsTest] Finishing Vector2 tests.");
    }

    // Vector3(x,y,z)
    void mathsTest::testVector3() {
        SPINDLE_TEST_INFO("[mathsTest] Starting Vector3 tests.");
        Vector3<float> vec3_a(1.0f, 2.0f, 2.0f);
        Vector3<float> vec3_b(3.0f, 4.0f, 5.0f);

        // Dot product tests
        float dotProductVector3 = vec3_a.dot(vec3_b);
        runTest(21.0f, dotProductVector3, "Dot Product of vec3_a and vec3_b");

        Vector3<float> vec3_c(5.0f, 6.0f, 7.0f);
        float dotProductTest1 = vec3_a.dot(vec3_c);
        runTest(31.0f, dotProductTest1, "Dot Product of vec3_a and vec3_c");

        Vector3<float> vec3_d(-1.0f, -2.0f, -2.0f);
        float dotProductTest2 = vec3_a.dot(vec3_d);
        runTest(-9.0f, dotProductTest2, "Dot Product of vec3_a and vec3_d");

        SPINDLE_TEST_INFO("[mathsTest] Finishing Vector3 tests.");
    }

    // Run all tests
    void mathsTest::runAllTests() {
        testBaseMaths();
        testVector2();
        testVector3();
    }
}
