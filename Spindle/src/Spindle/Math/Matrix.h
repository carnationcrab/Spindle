#pragma once

#include "../Core.h"
#include "SSE/SSE.h"
#include "AVX/AVX.h"

#include <array>
#include <cmath>
#include <string>
#include <sstream>

/**************************
*                         *
* generic matrix (NxM)    *
*                         *
**************************/

namespace Spindle {

    // all other types use scalar
    template <typename T, size_t N, size_t M>
    class Matrix {
    private:
        std::array<std::array<T, M>, N> data; // Row-major storage

    public:
        /**********************
        *    constructors     *
        **********************/
        constexpr Matrix() noexcept {
            for (size_t i = 0; i < N; ++i)
                for (size_t j = 0; j < M; ++j)
                    data[i][j] = T();
        }

        constexpr Matrix(const std::initializer_list<std::initializer_list<T>>& values) noexcept {
            size_t i = 0;
            for (const auto& row : values) {
                std::copy(row.begin(), row.end(), data[i].begin());
                ++i;
            }
        }

        /**********************
        *  accessors          *
        **********************/
        constexpr T& at(size_t row, size_t col) noexcept {
            return data[row][col];
        }

        constexpr const T& at(size_t row, size_t col) const noexcept {
            return data[row][col];
        }

        /**********************
        *  operator overloads *
        **********************/

        // addition
        constexpr Matrix<T, N, M> operator+(
            const Matrix<T, N, M>& operand) const noexcept {
            Matrix<T, N, M> result;

            for (size_t i = 0; i < N; ++i)
                for (size_t j = 0; j < M; ++j)
                    result.at(i, j) = this->at(i, j) + operand.at(i, j);
            
            return result;
        }

        // subtraction
        constexpr Matrix<T, N, M> operator-(
            const Matrix<T, N, M>& operand) const noexcept {
            Matrix<T, N, M> result;

            for (size_t i = 0; i < N; ++i)
                for (size_t j = 0; j < M; ++j)
                    result.at(i, j) = this->at(i, j) - operand.at(i, j);
            
            return result;
        }

        // scalar multiplication
        constexpr Matrix<T, N, M> operator*(T scalar) const noexcept {
            Matrix<T, N, M> result;
            for (size_t i = 0; i < N; ++i)
                for (size_t j = 0; j < M; ++j)
                    result.at(i, j) = this->at(i, j) * scalar;
            return result;
        }

        /**********************
        *       methods       *
        **********************/

        // transpose
        constexpr Matrix<T, M, N> transpose() const noexcept {
            Matrix<T, M, N> result;
            for (size_t i = 0; i < N; ++i)
                for (size_t j = 0; j < M; ++j)
                    result.at(j, i) = this->at(i, j);
            return result;
        }

        // string
        std::string toString() const noexcept {
            std::ostringstream oss;
            for (size_t i = 0; i < N; ++i) {
                oss << "[ ";
                for (size_t j = 0; j < M; ++j) {
                    oss << this->at(i, j) << " ";
                }
                oss << "]\n";
            }
            return oss.str();
        }
    };

    // floats use SIMD
    template <size_t N, size_t M>
    class Matrix<float, N, M> {
    private:
        alignas(16) std::array<std::array<float, M>, N> data; // Ensure SIMD alignment

    public:
        /**********************
        *    constructors     *
        **********************/
        Matrix() noexcept {
            for (size_t i = 0; i < N; ++i)
                for (size_t j = 0; j < M; ++j)
                    data[i][j] = 0.0f;
        }

        Matrix(const std::initializer_list<std::initializer_list<float>>& values) noexcept {
            size_t i = 0;
            for (const auto& row : values) {
                std::copy(row.begin(), row.end(), data[i].begin());
                ++i;
            }
        }

        /**********************
        *  accessors          *
        **********************/
        float& at(size_t row, size_t col) noexcept {
            return data[row][col];
        }

        const float& at(size_t row, size_t col) const noexcept {
            return data[row][col];
        }

        /**********************
        *  operator overloads *
        **********************/

        // SIMD addition
        Matrix<float, N, M> operator+(const Matrix<float, N, M>& operand) const noexcept {
#ifdef USE_AVX
            Matrix<float, N, M> result;
            for  (size_t i = 0; i < N; ++i) {
                __m256   a = AVX_Set(data[i][0], data[i][1], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                __m256   b = AVX_Set(operand.data[i][0], operand.data[i][1], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                __m256 res = AVX_Add(a, b);

                result.data[i][0] = AVX_GetX(res);
                result.data[i][1] = AVX_GetY(res);
            }
            return result;
#elif defined(USE_SSE)
            Matrix<float, N, M> result;
            for  (size_t i = 0; i < N; ++i) {
                __m128   a = SSE_Set(data[i][0], data[i][1], 0.0f, 0.0f);
                __m128   b = SSE_Set(operand.data[i][0], operand.data[i][1], 0.0f, 0.0f);
                __m128 res = SSE_Add(a, b);

                result.data[i][0] = SSE_GetX(res);
                result.data[i][1] = SSE_GetY(res);
            }
            return result;
#else
            Matrix<float, N, M> result;

            for (size_t i = 0; i < N; ++i)
                for (size_t j = 0; j < M; ++j)
                    result.data[i][j] = 
                    data[i][j] + operand.data[i][j];

            return result;
#endif
        }

        // SIMD subtraction
        Matrix<float, N, M> operator-(
            const Matrix<float, N, M>& operand) const noexcept {
#ifdef USE_AVX
            Matrix<float, N, M> result;

            for (size_t i = 0; i < N; ++i) {
                __m256   a = AVX_Set(data[i][0], data[i][1], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                __m256   b = AVX_Set(operand.data[i][0], operand.data[i][1], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                __m256 res = AVX_Subtract(a, b);

                result.data[i][0] = AVX_GetX(res);
                result.data[i][1] = AVX_GetY(res);
            }
            return result;

#elif defined(USE_SSE)
            Matrix<float, N, M> result;

            for (size_t i = 0; i < N; ++i) {
                __m128   a = SSE_Set(data[i][0], data[i][1], 0.0f, 0.0f);
                __m128   b = SSE_Set(operand.data[i][0], operand.data[i][1], 0.0f, 0.0f);
                __m128 res = SSE_Subtract(a, b);

                result.data[i][0] = SSE_GetX(res);
                result.data[i][1] = SSE_GetY(res);
            }
            return result;
#else
            Matrix<float, N, M> result;
            for (size_t i = 0; i < N; ++i)
                for (size_t j = 0; j < M; ++j)
                    result.data[i][j] = 
                    data[i][j] - operand.data[i][j];

            return result;
#endif
        }

        // multiplication to scalar
        Matrix<float, N, M> operator*(float scalar) const noexcept {
#ifdef USE_AVX
            Matrix<float, N, M> result;
            for  (size_t i = 0; i < N; ++i) {
                __m256   a = AVX_Set(data[i][0], data[i][1]);
                __m256 res = AVX_Multiply(a, scalar);

                result.data[i][0] = AVX_GetX(res);
                result.data[i][1] = AVX_GetY(res);
            }
            return result;
#elif defined(USE_SSE)
            Matrix<float, N, M> result;
            for  (size_t i = 0; i < N; ++i) {
                __m128   a = SSE_Set(data[i][0], data[i][1]);
                __m128 res = SSE_Multiply(a, scalar);
                result.data[i][0] = SSE_GetX(res);
                result.data[i][1] = SSE_GetY(res);
            }

            return result;

#else
            Matrix<float, N, M> result;
            for (size_t i = 0; i < N; ++i)
                for (size_t j = 0; j < M; ++j)
                    result.data[i][j] = data[i][j] * scalar;

            return result;
#endif
        }

        /**********************
        *       methods       *
        **********************/

        Matrix<float, M, N> transpose() const noexcept {

            Matrix<float, M, N> result;

            for (size_t i = 0; i < N; ++i)
                for (size_t j = 0; j < M; ++j)
                    result.at(j, i) = this->at(i, j);

            return result;
        }

        std::string toString() const noexcept {

            std::ostringstream oss;

            for (size_t i = 0; i < N; ++i) {
                oss << "[ ";
                for (size_t j = 0; j < M; ++j) {
                    oss << this->at(i, j) << " ";
                }
                oss << "]\n";
            }
            return oss.str();
        }
    };
}
