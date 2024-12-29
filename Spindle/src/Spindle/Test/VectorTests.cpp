#include "SpindleTest.h"
#include "../Math/Vector.h"

using namespace Spindle;

/**********************
*  Vector 2D Tests   *
**********************/

TEST_CASE(Vector2D_DefaultConstructor) {
    Vector<float, 2> v;
    SpindleTest::assertEqual(v.x, 0.0f, "Default constructor X should be 0.0f");
    SpindleTest::assertEqual(v.y, 0.0f, "Default constructor Y should be 0.0f");
}

TEST_CASE(Vector2D_ParameterizedConstructor) {
    Vector<float, 2> v(3.0f, 4.0f);
    SpindleTest::assertEqual(v.x, 3.0f, "Parameterized constructor X should be 3.0f");
    SpindleTest::assertEqual(v.y, 4.0f, "Parameterized constructor Y should be 4.0f");
}

TEST_CASE(Vector2D_Addition) {
    Vector<float, 2> v1(1.0f, 2.0f);
    Vector<float, 2> v2(3.0f, 4.0f);
    Vector<float, 2> result = v1 + v2;
    SpindleTest::assertEqual(result.x, 4.0f, "Addition X should be 4.0f");
    SpindleTest::assertEqual(result.y, 6.0f, "Addition Y should be 6.0f");
}

TEST_CASE(Vector2D_Subtraction) {
    Vector<float, 2> v1(5.0f, 6.0f);
    Vector<float, 2> v2(3.0f, 4.0f);
    Vector<float, 2> result = v1 - v2;
    SpindleTest::assertEqual(result.x, 2.0f, "Subtraction X should be 2.0f");
    SpindleTest::assertEqual(result.y, 2.0f, "Subtraction Y should be 2.0f");
}

TEST_CASE(Vector2D_DotProduct) {
    Vector<float, 2> v1(1.0f, 2.0f);
    Vector<float, 2> v2(3.0f, 4.0f);
    float result = v1.dot(v2);
    SpindleTest::assertEqual(result, 11.0f, "Dot product should be 11.0f");
}

TEST_CASE(Vector2D_Magnitude) {
    Vector<float, 2> v(3.0f, 4.0f);
    float result = v.magnitude();
    SpindleTest::assertEqual(result, 5.0f, "Magnitude should be 5.0f");
}

TEST_CASE(Vector2D_UnitVector) {
    Vector<float, 2> v(3.0f, 4.0f);
    Vector<float, 2> result = v.unitVector();
    SpindleTest::assertEqual(result.x, 0.6f, "Unit vector X should be 0.6f");
    SpindleTest::assertEqual(result.y, 0.8f, "Unit vector Y should be 0.8f");
}

TEST_CASE(Vector2D_ToString) {
    Vector<float, 2> v(1.0f, 2.0f);
    SpindleTest::assertTrue(v.ToString() == "(1.000000, 2.000000)", "ToString output should be (1.000000, 2.000000)");
}

/**********************
*  Vector 3D Tests   *
**********************/

TEST_CASE(Vector3D_DefaultConstructor) {
    Vector<float, 3> v;
    SpindleTest::assertEqual(v.x, 0.0f, "Default constructor X should be 0.0f");
    SpindleTest::assertEqual(v.y, 0.0f, "Default constructor Y should be 0.0f");
    SpindleTest::assertEqual(v.z, 0.0f, "Default constructor Z should be 0.0f");
}

TEST_CASE(Vector3D_ParameterizedConstructor) {
    Vector<float, 3> v(3.0f, 4.0f, 5.0f);
    SpindleTest::assertEqual(v.x, 3.0f, "Parameterized constructor X should be 3.0f");
    SpindleTest::assertEqual(v.y, 4.0f, "Parameterized constructor Y should be 4.0f");
    SpindleTest::assertEqual(v.z, 5.0f, "Parameterized constructor Z should be 5.0f");
}

TEST_CASE(Vector3D_Addition) {
    Vector<float, 3> v1(1.0f, 2.0f, 3.0f);
    Vector<float, 3> v2(4.0f, 5.0f, 6.0f);
    Vector<float, 3> result = v1 + v2;
    SpindleTest::assertEqual(result.x, 5.0f, "Addition X should be 5.0f");
    SpindleTest::assertEqual(result.y, 7.0f, "Addition Y should be 7.0f");
    SpindleTest::assertEqual(result.z, 9.0f, "Addition Z should be 9.0f");
}

TEST_CASE(Vector3D_Subtraction) {
    Vector<float, 3> v1(7.0f, 8.0f, 9.0f);
    Vector<float, 3> v2(4.0f, 5.0f, 6.0f);
    Vector<float, 3> result = v1 - v2;
    SpindleTest::assertEqual(result.x, 3.0f, "Subtraction X should be 3.0f");
    SpindleTest::assertEqual(result.y, 3.0f, "Subtraction Y should be 3.0f");
    SpindleTest::assertEqual(result.z, 3.0f, "Subtraction Z should be 3.0f");
}

TEST_CASE(Vector3D_DotProduct) {
    Vector<float, 3> v1(1.0f, 2.0f, 3.0f);
    Vector<float, 3> v2(4.0f, 5.0f, 6.0f);
    float result = v1.dot(v2);
    SpindleTest::assertEqual(result, 32.0f, "Dot product should be 32.0f");
}

TEST_CASE(Vector3D_CrossProduct) {
    Vector<float, 3> v1(1.0f, 0.0f, 0.0f);
    Vector<float, 3> v2(0.0f, 1.0f, 0.0f);
    Vector<float, 3> result = v1.cross(v2);
    SpindleTest::assertEqual(result.x, 0.0f, "Cross product X should be 0.0f");
    SpindleTest::assertEqual(result.y, 0.0f, "Cross product Y should be 0.0f");
    SpindleTest::assertEqual(result.z, 1.0f, "Cross product Z should be 1.0f");
}

TEST_CASE(Vector3D_Magnitude) {
    Vector<float, 3> v(1.0f, 2.0f, 2.0f);
    float result = v.magnitude();
    SpindleTest::assertEqual(result, 3.0f, "Magnitude should be 3.0f");
}

TEST_CASE(Vector3D_UnitVector) {
    Vector<float, 3> v(0.0f, 3.0f, 4.0f);
    Vector<float, 3> result = v.unitVector();
    SpindleTest::assertEqual(result.x, 0.0f, "Unit vector X should be 0.0f");
    SpindleTest::assertEqual(result.y, 0.6f, "Unit vector Y should be 0.6f");
    SpindleTest::assertEqual(result.z, 0.8f, "Unit vector Z should be 0.8f");
}

TEST_CASE(Vector3D_ToString) {
    Vector<float, 3> v(1.0f, 2.0f, 3.0f);
    SpindleTest::assertTrue(v.ToString() == "(1.000000, 2.000000, 3.000000)", "ToString output should be (1.000000, 2.000000, 3.000000)");
}