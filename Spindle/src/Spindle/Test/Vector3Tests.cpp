#include "TestFramework.h"
#include "../Math/Vector3.h"

using namespace Spindle;

TEST_CASE(Vector3_DefaultConstructor) {
    Vector3<float> v;
    return TestFramework::assertEqual(v.getX(), 0.0f, "Default constructor X should be 0.0f")
        && TestFramework::assertEqual(v.getY(), 0.0f, "Default constructor Y should be 0.0f")
        && TestFramework::assertEqual(v.getZ(), 0.0f, "Default constructor Z should be 0.0f");
}

TEST_CASE(Vector3_ParameterizedConstructor) {
    Vector3<float> v(3.0f, 4.0f, 5.0f);
    return TestFramework::assertEqual(v.getX(), 3.0f, "Parameterized constructor X should be 3.0f")
        && TestFramework::assertEqual(v.getY(), 4.0f, "Parameterized constructor Y should be 4.0f")
        && TestFramework::assertEqual(v.getZ(), 5.0f, "Parameterized constructor Z should be 5.0f");
}

TEST_CASE(Vector3_Addition) {
    Vector3<float> v1(1.0f, 2.0f, 3.0f);
    Vector3<float> v2(3.0f, 4.0f, 5.0f);
    Vector3<float> result = v1 + v2;
    return TestFramework::assertEqual(result.getX(), 4.0f, "Addition X should be 4.0f")
        && TestFramework::assertEqual(result.getY(), 6.0f, "Addition Y should be 6.0f")
        && TestFramework::assertEqual(result.getZ(), 8.0f, "Addition Z should be 8.0f");
}

TEST_CASE(Vector3_Subtraction) {
    Vector3<float> v1(5.0f, 6.0f, 7.0f);
    Vector3<float> v2(3.0f, 4.0f, 5.0f);
    Vector3<float> result = v1 - v2;
    return TestFramework::assertEqual(result.getX(), 2.0f, "Subtraction X should be 2.0f")
        && TestFramework::assertEqual(result.getY(), 2.0f, "Subtraction Y should be 2.0f")
        && TestFramework::assertEqual(result.getZ(), 2.0f, "Subtraction Z should be 2.0f");
}

TEST_CASE(Vector3_ScalarMultiplication) {
    Vector3<float> v(1.5f, -2.5f, 3.5f);
    Vector3<float> result = v * 2.0f;
    return TestFramework::assertEqual(result.getX(), 3.0f, "Multiplication X should be 3.0f")
        && TestFramework::assertEqual(result.getY(), -5.0f, "Multiplication Y should be -5.0f")
        && TestFramework::assertEqual(result.getZ(), 7.0f, "Multiplication Z should be 7.0f");
}

TEST_CASE(Vector3_DotProduct) {
    Vector3<float> v1(3.0f, 4.0f, 5.0f);
    Vector3<float> v2(-1.0f, 2.0f, 3.0f);
    float result = v1.dot(v2);
    return TestFramework::assertEqual(result, 20.0f, "Dot product should be 26.0f");
}

TEST_CASE(Vector3_MagnitudeSquared) {
    Vector3<float> v(3.0f, 4.0f, 5.0f);
    float result = v.magnitudeSquared();
    return TestFramework::assertEqual(result, 50.0f, "Magnitude squared should be 50.0f");
}

TEST_CASE(Vector3_Magnitude) {
    Vector3<float> v(3.0f, 4.0f, 5.0f);
    float result = v.magnitude();
    return TestFramework::assertEqual(result, std::sqrt(50.0f), "Magnitude should be sqrt(50.0f)");
}

TEST_CASE(Vector3_CrossProduct) {
    Vector3<float> v1(1.0f, 0.0f, 0.0f);
    Vector3<float> v2(0.0f, 1.0f, 0.0f);
    Vector3<float> result = v1.cross(v2);
    return TestFramework::assertEqual(result.getX(), 0.0f, "Cross product X should be 0.0f")
        && TestFramework::assertEqual(result.getY(), 0.0f, "Cross product Y should be 0.0f")
        && TestFramework::assertEqual(result.getZ(), 1.0f, "Cross product Z should be 1.0f");
}

TEST_CASE(Vector3_ToString) {
    Vector3<float> v(1.0f, 2.0f, 3.0f);
    return TestFramework::assertTrue(v.toString() == "(1.000000, 2.000000, 3.000000)", "toString output should be (1.000000, 2.000000, 3.000000)");
}
