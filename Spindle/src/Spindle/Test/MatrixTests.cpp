#include "SpindleTest.h"
#include "../Math/Matrix.h"

using namespace Spindle;

// 2x2 matrix
TEST_CASE(Matrix2x2_DefaultConstructor) {
    Matrix<int, 2, 2> mat;
    SpindleTest::assertEqual(mat.at(0, 0), 0, "Default element at (0,0) should be 0");
    SpindleTest::assertEqual(mat.at(1, 1), 0, "Default element at (1,1) should be 0");
}

TEST_CASE(Matrix2x2_ParameterizedConstructor) {
    Matrix<float, 2, 2> mat({
        {1.0f, 2.0f},
        {3.0f, 4.0f}
        });

    SpindleTest::assertEqual(mat.at(0, 0), 1.0f, "Element at (0,0) should be 1.0f");
    SpindleTest::assertEqual(mat.at(1, 1), 4.0f, "Element at (1,1) should be 4.0f");
}

TEST_CASE(Matrix2x2_Addition) {
    Matrix<int, 2, 2> mat1({
        {1, 2},
        {3, 4}
        });
    Matrix<int, 2, 2> mat2({
        {5, 6},
        {7, 8}
        });

    Matrix<int, 2, 2> result = mat1 + mat2;

    SpindleTest::assertEqual(result.at(0, 0), 6, "Addition result at (0,0) should be 6");
    SpindleTest::assertEqual(result.at(1, 1), 12, "Addition result at (1,1) should be 12");
}

TEST_CASE(Matrix2x2_Subtraction) {
    Matrix<float, 2, 2> mat1({
        {5.0f, 6.0f},
        {7.0f, 8.0f}
        });
    Matrix<float, 2, 2> mat2({
        {1.0f, 2.0f},
        {3.0f, 4.0f}
        });

    Matrix<float, 2, 2> result = mat1 - mat2;

    SpindleTest::assertEqual(result.at(0, 0), 4.0f, "Subtraction result at (0,0) should be 4.0f");
    SpindleTest::assertEqual(result.at(1, 1), 4.0f, "Subtraction result at (1,1) should be 4.0f");
}

// 3x3 matrix
TEST_CASE(Matrix3x3_ParameterizedConstructor) {
    Matrix<float, 3, 3> mat({
        {1.0f, 2.0f, 3.0f},
        {4.0f, 5.0f, 6.0f},
        {7.0f, 8.0f, 9.0f}
        });

    SpindleTest::assertEqual(mat.at(0, 0), 1.0f, "Element at (0,0) should be 1.0f");
    SpindleTest::assertEqual(mat.at(2, 2), 9.0f, "Element at (2,2) should be 9.0f");
}

TEST_CASE(Matrix3x3_Addition) {
    Matrix<int, 3, 3> mat1({
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
        });
    Matrix<int, 3, 3> mat2({
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
        });

    Matrix<int, 3, 3> result = mat1 + mat2;

    SpindleTest::assertEqual(result.at(0, 0), 10, "Addition result at (0,0) should be 10");
    SpindleTest::assertEqual(result.at(2, 2), 10, "Addition result at (2,2) should be 10");
}

TEST_CASE(Matrix3x3_Transpose) {
    Matrix<int, 3, 3> mat({
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
        });

    Matrix<int, 3, 3> result = mat.transpose();


    SpindleTest::assertEqual(result.at(0, 1), 4, "Transpose result at (0,1) should be 4");
    SpindleTest::assertEqual(result.at(2, 0), 3, "Transpose result at (2,0) should be 3");
}

// 4x4 matrix
TEST_CASE(Matrix4x4_DefaultConstructor) {
    Matrix<float, 4, 4> mat;

    SpindleTest::assertEqual(mat.at(0, 0), 0.0f, "Default element at (0,0) should be 0.0f");
    SpindleTest::assertEqual(mat.at(3, 3), 0.0f, "Default element at (3,3) should be 0.0f");
}

TEST_CASE(Matrix4x4_ParameterizedConstructor) {
    Matrix<float, 4, 4> mat({
        {1.0f, 2.0f, 3.0f, 4.0f},
        {5.0f, 6.0f, 7.0f, 8.0f},
        {9.0f, 10.0f, 11.0f, 12.0f},
        {13.0f, 14.0f, 15.0f, 16.0f}
        });

    SpindleTest::assertEqual(mat.at(0, 0), 1.0f, "Element at (0,0) should be 1.0f");
    SpindleTest::assertEqual(mat.at(3, 3), 16.0f, "Element at (3,3) should be 16.0f");
}

TEST_CASE(Matrix4x4_Addition) {
    Matrix<int, 4, 4> mat1({
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
        });
    Matrix<int, 4, 4> mat2({
        {16, 15, 14, 13},
        {12, 11, 10, 9},
        {8, 7, 6, 5},
        {4, 3, 2, 1}
        });

    Matrix<int, 4, 4> result = mat1 + mat2;

    SpindleTest::assertEqual(result.at(0, 0), 17, "Addition result at (0,0) should be 17");
    SpindleTest::assertEqual(result.at(3, 3), 17, "Addition result at (3,3) should be 17");
}

TEST_CASE(Matrix4x4_ScalarMultiplication) {
    Matrix<int, 4, 4> mat({
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
        });

    Matrix<int, 4, 4> result = mat * 2;

    SpindleTest::assertEqual(result.at(0, 0), 2, "Scalar multiplication result at (0,0) should be 2");
    SpindleTest::assertEqual(result.at(3, 3), 32, "Scalar multiplication result at (3,3) should be 32");
}

// 4x3 matrix (used for transformations in 3D)
TEST_CASE(Matrix4x3_Transpose) {
    Matrix<int, 4, 3> mat({
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
        {10, 11, 12}
        });

    Matrix<int, 3, 4> result = mat.transpose();

    SpindleTest::assertEqual(result.at(0, 1), 4, "Transpose result at (0,1) should be 4");
    SpindleTest::assertEqual(result.at(2, 3), 12, "Transpose result at (2,3) should be 12");
}
