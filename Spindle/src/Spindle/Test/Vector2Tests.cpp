#include "TestFramework.h"
#include "../Math/Vector2.h"

using namespace Spindle;

TEST_CASE(Vector2_DefaultConstructor) {
    Vector2<float> v;
    return TestFramework::assertEqual(v.getX(), 0.0f, "Default constructor X should be 0.0f")
        && TestFramework::assertEqual(v.getY(), 0.0f, "Default constructor Y should be 0.0f");
}

TEST_CASE(Vector2_ParameterizedConstructor) {
    Vector2<float> v(3.0f, 4.0f);
    return TestFramework::assertEqual(v.getX(), 3.0f, "Parameterized constructor X should be 3.0f")
        && TestFramework::assertEqual(v.getY(), 4.0f, "Parameterized constructor Y should be 4.0f");
}

TEST_CASE(Vector2_Addition) {
    Vector2<float> v1(1.0f, 2.0f);
    Vector2<float> v2(3.0f, 4.0f);
    Vector2<float> result = v1 + v2;
    return TestFramework::assertEqual(result.getX(), 4.0f, "Addition X should be 4.0f") 
        && TestFramework::assertEqual(result.getY(), 6.0f, "Addition Y should be 6.0f");
}

TEST_CASE(Vector2_Subtraction) {
    Vector2<float> v1(5.0f, 6.0f);
    Vector2<float> v2(3.0f, 4.0f);
    Vector2<float> result = v1 - v2;
    return TestFramework::assertEqual(result.getX(), 2.0f, "Subtraction X should be 2.0f")
        && TestFramework::assertEqual(result.getY(), 2.0f, "Subtraction Y should be 2.0f");
}

TEST_CASE(Vector2_ScalarMultiplication) {
    Vector2<float> v(1.5f, -2.5f);
    Vector2<float> result = v * 2.0f;
    return TestFramework::assertEqual(result.getX(), 3.0f, "Multiplication X should be 3.0f")
        && TestFramework::assertEqual(result.getY(), -5.0f, "Multiplication Y should be -5.0f");
}

TEST_CASE(Vector2_DotProduct) {
    Vector2<float> v1(3.0f, 4.0f);
    Vector2<float> v2(-1.0f, 2.0f);
    float result = v1.dot(v2);
    return TestFramework::assertEqual(result, 5.0f, "Dot product");
}

TEST_CASE(Vector2_ToString) {
    Vector2<float> v(1.0f, 2.0f);
    return TestFramework::assertTrue(v.toString() == "(1.000000, 2.000000)", "toString output should be (1.000000, 2.000000)");
}
