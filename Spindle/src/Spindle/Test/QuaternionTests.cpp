#include "SpindleTest.h"
#include "../Math/Quaternion.h"
#include <cmath>

using namespace Spindle;

// Basic Construction Tests
TEST_CASE(QuaternionInt_DefaultConstructor) {
    Quaternion<int> q;
    SpindleTest::assertEqual(q.getX(), 0, "Default X should be 0");
    SpindleTest::assertEqual(q.getY(), 0, "Default Y should be 0");
    SpindleTest::assertEqual(q.getZ(), 0, "Default Z should be 0");
    SpindleTest::assertEqual(q.getW(), 1, "Default W should be 1");
}

TEST_CASE(QuaternionFloat_DefaultConstructor) {
    Quaternion<float> q;
    SpindleTest::assertEqual(q.getX(), 0.0f, "Default X should be 0.0f");
    SpindleTest::assertEqual(q.getY(), 0.0f, "Default Y should be 0.0f");
    SpindleTest::assertEqual(q.getZ(), 0.0f, "Default Z should be 0.0f");
    SpindleTest::assertEqual(q.getW(), 1.0f, "Default W should be 1.0f");
}

TEST_CASE(QuaternionFloat_ParameterizedConstructor) {
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    SpindleTest::assertEqual(q.getX(), 1.0f, "X should be 1.0f");
    SpindleTest::assertEqual(q.getY(), 2.0f, "Y should be 2.0f");
    SpindleTest::assertEqual(q.getZ(), 3.0f, "Z should be 3.0f");
    SpindleTest::assertEqual(q.getW(), 4.0f, "W should be 4.0f");
}

// Arithmetic Operation Tests
TEST_CASE(QuaternionInt_Addition) {
    Quaternion<int> q1(1, 2, 3, 4);
    Quaternion<int> q2(5, 6, 7, 8);
    Quaternion<int> result = q1 + q2;

    SpindleTest::assertEqual(result.getX(), 6, "X after addition should be 6");
    SpindleTest::assertEqual(result.getY(), 8, "Y after addition should be 8");
    SpindleTest::assertEqual(result.getZ(), 10, "Z after addition should be 10");
    SpindleTest::assertEqual(result.getW(), 12, "W after addition should be 12");
}

TEST_CASE(QuaternionFloat_Addition) {
    Quaternion<float> q1(1.5f, 2.5f, 3.5f, 4.5f);
    Quaternion<float> q2(0.5f, 1.5f, 2.5f, 3.5f);
    Quaternion<float> result = q1 + q2;

    SpindleTest::assertEqual(result.getX(), 2.0f, "X after addition should be 2.0f");
    SpindleTest::assertEqual(result.getY(), 4.0f, "Y after addition should be 4.0f");
    SpindleTest::assertEqual(result.getZ(), 6.0f, "Z after addition should be 6.0f");
    SpindleTest::assertEqual(result.getW(), 8.0f, "W after addition should be 8.0f");
}

TEST_CASE(QuaternionInt_Subtraction) {
    Quaternion<int> q1(5, 6, 7, 8);
    Quaternion<int> q2(1, 2, 3, 4);
    Quaternion<int> result = q1 - q2;

    SpindleTest::assertEqual(result.getX(), 4, "X after subtraction should be 4");
    SpindleTest::assertEqual(result.getY(), 4, "Y after subtraction should be 4");
    SpindleTest::assertEqual(result.getZ(), 4, "Z after subtraction should be 4");
    SpindleTest::assertEqual(result.getW(), 4, "W after subtraction should be 4");
}

TEST_CASE(QuaternionFloat_Subtraction) {
    Quaternion<float> q1(5.5f, 6.5f, 7.5f, 8.5f);
    Quaternion<float> q2(1.5f, 2.5f, 3.5f, 4.5f);
    Quaternion<float> result = q1 - q2;

    SpindleTest::assertEqual(result.getX(), 4.0f, "X after subtraction should be 4.0f");
    SpindleTest::assertEqual(result.getY(), 4.0f, "Y after subtraction should be 4.0f");
    SpindleTest::assertEqual(result.getZ(), 4.0f, "Z after subtraction should be 4.0f");
    SpindleTest::assertEqual(result.getW(), 4.0f, "W after subtraction should be 4.0f");
}

TEST_CASE(QuaternionInt_HamiltonProduct) {
    Quaternion<int> q1(1, 2, 3, 4);
    Quaternion<int> q2(5, 6, 7, 8);
    Quaternion<int> result = q1 * q2;

    // Correct Hamilton product calculations:
    int expectedX = 4 * 5 + 1 * 8 + 2 * 7 - 3 * 6;  // w1*x2 + x1*w2 + y1*z2 - z1*y2
    int expectedY = 4 * 6 - 1 * 7 + 2 * 8 + 3 * 5;  // w1*y2 - x1*z2 + y1*w2 + z1*x2
    int expectedZ = 4 * 7 + 1 * 6 - 2 * 5 + 3 * 8;  // w1*z2 + x1*y2 - y1*x2 + z1*w2
    int expectedW = 4 * 8 - 1 * 5 - 2 * 6 - 3 * 7;  // w1*w2 - x1*x2 - y1*y2 - z1*z2

    SpindleTest::assertEqual(result.getX(), expectedX, "X after Hamilton product");
    SpindleTest::assertEqual(result.getY(), expectedY, "Y after Hamilton product");
    SpindleTest::assertEqual(result.getZ(), expectedZ, "Z after Hamilton product");
    SpindleTest::assertEqual(result.getW(), expectedW, "W after Hamilton product");
}

TEST_CASE(QuaternionFloat_HamiltonProduct) {
    Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);     // (x1, y1, z1, w1)
    Quaternion<float> q2(5.0f, 6.0f, 7.0f, 8.0f);     // (x2, y2, z2, w2)
    Quaternion<float> result = q1 * q2;

    float expectedX = 4.0f * 5.0f + 1.0f * 8.0f + 2.0f * 7.0f - 3.0f * 6.0f;  // w1*x2 + x1*w2 + y1*z2 - z1*y2
    float expectedY = 4.0f * 6.0f - 1.0f * 7.0f + 2.0f * 8.0f + 3.0f * 5.0f;  // w1*y2 - x1*z2 + y1*w2 + z1*x2
    float expectedZ = 4.0f * 7.0f + 1.0f * 6.0f - 2.0f * 5.0f + 3.0f * 8.0f;  // w1*z2 + x1*y2 - y1*x2 + z1*w2
    float expectedW = 4.0f * 8.0f - 1.0f * 5.0f - 2.0f * 6.0f - 3.0f * 7.0f;  // w1*w2 - x1*x2 - y1*y2 - z1*z2

    SpindleTest::assertEqual(result.getX(), expectedX, "X after Hamilton product");
    SpindleTest::assertEqual(result.getY(), expectedY, "Y after Hamilton product");
    SpindleTest::assertEqual(result.getZ(), expectedZ, "Z after Hamilton product");
    SpindleTest::assertEqual(result.getW(), expectedW, "W after Hamilton product");
}

// Add a test with identity quaternion to verify multiplication properties
TEST_CASE(QuaternionFloat_HamiltonProduct_Identity) {
    Quaternion<float> identity(0.0f, 0.0f, 0.0f, 1.0f);  // Identity quaternion
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);

    Quaternion<float> result1 = q * identity;
    Quaternion<float> result2 = identity * q;

    // q * identity = identity * q = q
    SpindleTest::assertEqual(result1.getX(), q.getX(), "Identity right multiplication X");
    SpindleTest::assertEqual(result1.getY(), q.getY(), "Identity right multiplication Y");
    SpindleTest::assertEqual(result1.getZ(), q.getZ(), "Identity right multiplication Z");
    SpindleTest::assertEqual(result1.getW(), q.getW(), "Identity right multiplication W");

    SpindleTest::assertEqual(result2.getX(), q.getX(), "Identity left multiplication X");
    SpindleTest::assertEqual(result2.getY(), q.getY(), "Identity left multiplication Y");
    SpindleTest::assertEqual(result2.getZ(), q.getZ(), "Identity left multiplication Z");
    SpindleTest::assertEqual(result2.getW(), q.getW(), "Identity left multiplication W");
}

// Additional Tests for Magnitude, Normalization, and Other Operations
TEST_CASE(QuaternionFloat_Magnitude) {
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    float expectedMagnitude = std::sqrt(30.0f); // 1² + 2² + 3² + 4² = 30
    SpindleTest::assertEqual(q.magnitude(), expectedMagnitude, "Magnitude calculation");
}

TEST_CASE(QuaternionFloat_Normalize) {
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    float mag = std::sqrt(30.0f);
    Quaternion<float> result = q.normalize();

    SpindleTest::assertEqual(result.getX(), 1.0f / mag, "Normalized X", MEDIUM_EPSILON);
    SpindleTest::assertEqual(result.getY(), 2.0f / mag, "Normalized Y", MEDIUM_EPSILON);
    SpindleTest::assertEqual(result.getZ(), 3.0f / mag, "Normalized Z", MEDIUM_EPSILON);
    SpindleTest::assertEqual(result.getW(), 4.0f / mag, "Normalized W", MEDIUM_EPSILON);
}

TEST_CASE(QuaternionFloat_NormalizeZero) {
    Quaternion<float> q(0.0f, 0.0f, 0.0f, 0.0f);
    Quaternion<float> result = q.normalize();

    SpindleTest::assertEqual(result.getX(), 0.0f, "Zero quaternion X should remain 0");
    SpindleTest::assertEqual(result.getY(), 0.0f, "Zero quaternion Y should remain 0");
    SpindleTest::assertEqual(result.getZ(), 0.0f, "Zero quaternion Z should remain 0");
    SpindleTest::assertEqual(result.getW(), 0.0f, "Zero quaternion W should remain 0");
}

TEST_CASE(QuaternionInt_Conjugate) {
    Quaternion<int> q(1, 2, 3, 4);
    Quaternion<int> result = q.conjugate();

    SpindleTest::assertEqual(result.getX(), -1, "Conjugate X");
    SpindleTest::assertEqual(result.getY(), -2, "Conjugate Y");
    SpindleTest::assertEqual(result.getZ(), -3, "Conjugate Z");
    SpindleTest::assertEqual(result.getW(), 4, "Conjugate W should remain unchanged");
}

TEST_CASE(QuaternionFloat_Dot) {
    Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion<float> q2(5.0f, 6.0f, 7.0f, 8.0f);
    float result = q1.dot(q2);
    float expected = 1.0f * 5.0f + 2.0f * 6.0f + 3.0f * 7.0f + 4.0f * 8.0f;

    SpindleTest::assertEqual(result, expected, "Dot product calculation");
}

TEST_CASE(QuaternionFloat_Inverse) {
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    float normSquared = 30.0f; // 1² + 2² + 3² + 4² = 30
    Quaternion<float> result = q.inverse();

    SpindleTest::assertEqual(result.getX(), -1.0f / normSquared, "Inverse X");
    SpindleTest::assertEqual(result.getY(), -2.0f / normSquared, "Inverse Y");
    SpindleTest::assertEqual(result.getZ(), -3.0f / normSquared, "Inverse Z");
    SpindleTest::assertEqual(result.getW(), 4.0f / normSquared, "Inverse W");
}