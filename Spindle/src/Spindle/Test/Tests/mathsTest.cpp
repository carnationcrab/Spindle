#include "mathsTest.h"
#include "../../Log.h"

namespace Spindle {

    // Scalar Maths
    void mathsTest::testBaseMaths() {
        // SQRT(x,y);
        SPINDLE_CORE_INFO("[mathsTest] Starting base math tests."); 
        std::cout << "\nTesting SQRT:\n";
        std::cout << "SQRT(4.0) = " << SQRT(4.0) << " (Expected: 2.0)\n";
        std::cout << "SQRT(9.0) = " << SQRT(9.0) << " (Expected: 3.0)\n";

        // POWR(base,exponent);
        std::cout << "\nTesting POWR:\n";
        std::cout << "POWR(2.0, 3.0) = " << POWR(2.0, 3.0) << " (Expected: 8.0)\n";
        std::cout << "POWR(3.0, 2.0) = " << POWR(3.0, 2.0) << " (Expected: 9.0)\n";

        // SIN();
        std::cout << "\nTesting SIN:\n";
        std::cout << "SIN(0.0) = " << SIN(0.0) << " (Expected: 0.0)\n";
        std::cout << "SIN(3.14159 / 2) = " << SIN(3.14159 / 2) << " (Expected: ~1.0)\n";
    }

    // Vector2(x,y)
    void mathsTest::testVector2() {
        SPINDLE_CORE_INFO("[mathsTest] Starting Vector2 tests.");
        Vector2<float> vec2_a(3.0f, 4.0f);
        Vector2<float> vec2_b(1.0f, 2.0f);

        std::cout << "\nTesting Vector2:" << std::endl;
        vec2_a.print(); // Output: Vector2(3, 4) | Magnitude: 5
        vec2_b.print(); // Output: Vector2(1, 2) | Magnitude: 2.23607

        auto sumVector2 = vec2_a + vec2_b;
        sumVector2.print(); // Output: Vector2(4, 6) | Magnitude: 7.2111

        auto diffVector2 = vec2_a - vec2_b;
        diffVector2.print(); // Output: Vector2(2, 2) | Magnitude: 2.82843

        float dotProductVector2 = vec2_a.dot(vec2_b);
        std::cout << "Testing Dot Product of vec2_a and vec2_b: " << dotProductVector2 << std::endl; // Output: 11
        SPINDLE_CORE_INFO("[mathsTest] Finishing Vector2 tests.");
    }

    // 3D Vector Tests
    void mathsTest::testVector3() {
        SPINDLE_CORE_INFO("[mathsTest] Starting Vector3 tests.");
        Vector3<float> vec3_a(1.0f, 2.0f, 2.0f);
        Vector3<float> vec3_b(3.0f, 4.0f, 5.0f);

        std::cout << "\nTesting Vector3:" << std::endl;
        vec3_a.print(); // Output: Vector3(1, 2, 2) | Magnitude: 3
        vec3_b.print(); // Output: Vector3(3, 4, 5) | Magnitude: 7.07107

        auto sumVector3 = vec3_a + vec3_b;
        sumVector3.print(); // Output: Vector3(4, 6, 7) | Magnitude: 9.16515

        auto diffVector3 = vec3_a - vec3_b;
        diffVector3.print(); // Output: Vector3(-2, -2, -3) | Magnitude: 3.74166
        SPINDLE_CORE_INFO("[mathsTest] Finishing Vector3 tests.");
    }

    void mathsTest::runAllTests() {
        testBaseMaths();
        testVector2();
        testVector3();
    }
}
