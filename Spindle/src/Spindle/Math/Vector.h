#pragma once

#include "../Core.h"
#include "SSE/SSE.h"
#include "AVX/AVX.h"

#include <cmath>
#include <string>

/**************************
*                         *
*         vector          *
*                         *
**************************/

namespace Spindle {

    template <typename T, size_t Dimension>
    struct Vector;

    // all other  types use scalar
    template <typename T, size_t Dimension>
    struct Vector {
        T coordinates[Dimension];

        /**********************
        *    constructors     *
        **********************/

        constexpr Vector() noexcept {
            for (size_t i = 0; i < Dimension; ++i) {
                coordinates[i] = T();
            }
        }

        Vector(const T(&coords)[Dimension]) {
            for (size_t i = 0; i < Dimension; ++i) {
                coordinates[i] = coords[i];
            }
        }

        /**********************
        *  operator overloads *
        **********************/

        Vector operator+(const Vector& other) const {
            T result[Dimension];
            for (size_t i = 0; i < Dimension; ++i) {
                result[i] = coordinates[i] + other.coordinates[i];
            }
            return Vector(result);
        }

        Vector operator-(const Vector& other) const {
            T result[Dimension];
            for (size_t i = 0; i < Dimension; ++i) {
                result[i] = coordinates[i] - other.coordinates[i];
            }
            return Vector(result);
        }

        Vector operator*(T scalar) const {
            T result[Dimension];
            for (size_t i = 0; i < Dimension; ++i) {
                result[i] = coordinates[i] * scalar;
            }
            return Vector(result);
        }

        bool operator==(const Vector& other) const noexcept {
            for (size_t i = 0; i < Dimension; ++i) {
                if (components[i] != other.components[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const Vector& other) const noexcept {
            return !(*this == other);
        }


        /**********************
        *       methods       *
        **********************/

        T dot(const Vector& other) const {
            T sum = T();
            for (size_t i = 0; i < Dimension; ++i) {
                sum += coordinates[i] * other.coordinates[i];
            }
            return sum;
        }

        T magnitudeSquared() const {
            return dot(*this);
        }

        T magnitude() const {
            return std::sqrt(magnitudeSquared());
        }

        Vector unitVector() const {
            T mag = magnitude();
            T result[Dimension];
            for (size_t i = 0; i < Dimension; ++i) {
                result[i] = coordinates[i] / mag;
            }
            return Vector(result);
        }

        std::string ToString() const {
            std::string result = "(";
            for (size_t i = 0; i < Dimension; ++i) {
                result += std::to_string(coordinates[i]);
                if (i < Dimension - 1) result += ", ";
            }
            result += ")";
            return result;
        }
    };

    // floats use SIMD
    template <>
    struct Vector<float, 2> {
        float x, y;

        /**********************
        *    constructors     *
        **********************/

        constexpr Vector() noexcept : x(0.0f), y(0.0f) {}

        Vector(float px, float py) noexcept : x(px), y(py) {}

        /**********************
        *  operator overloads *
        **********************/

        Vector operator+(const Vector& operand) const noexcept {
#ifdef USE_AVX
            __m256      a = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256      b = AVX_Set(operand.x, operand.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 result = AVX_Add(a, b);

            return setVector(result);

#elif defined(USE_SSE)
            __m128      a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128      b = SSE_Set(operand.x, operand.y, 0.0f, 0.0f);
            __m128 result = SSE_Add(a, b);

            return setVector(result);
#else
            return Vector(x + operand.x, y + operand.y);
#endif
        }

        Vector operator-(const Vector& operand) const noexcept {
#ifdef USE_AVX
            __m256      a = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256      b = AVX_Set(operand.x, operand.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 result = AVX_Subtract(a, b);

            return setVector(result);
#elif defined(USE_SSE)
            __m128      a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128      b = SSE_Set(operand.x, operand.y, 0.0f, 0.0f);
            __m128 result = SSE_Subtract(a, b);
            
            return setVector(result);
#else
            return Vector(x - operand.x, y - operand.y);
#endif
        }

        Vector operator*(float scalar) const noexcept {
#ifdef USE_AVX
            __m256      a = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 result = AVX_Multiply(a, scalar);
           
            return setVector(result);

#elif defined(USE_SSE)
            __m128      a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 result = SSE_Multiply(a, scalar);

            return setVector(result);
#else
            return Vector(x * scalar, y * scalar);
#endif
        }

        bool operator==(const Vector<float, 2>& other) const noexcept {
#ifdef USE_AVX
            __m256 a = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 b = AVX_Set(other.x, other.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            return AVX_AllTrue(AVX_CompareEqual(a, b));
#elif defined(USE_SSE)
            __m128 a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 b = SSE_Set(other.x, other.y, 0.0f, 0.0f);
            return SSE_AllTrue(SSE_CompareEqual(a, b));
#else
            return x == other.x && y == other.y;
#endif
        }

        // Inequality operator
        bool operator!=(const Vector<float, 2>& other) const noexcept {
            return !(*this == other);
        }

        /**********************
        *       methods       *
        **********************/

        Vector unitVector() const noexcept {
#ifdef USE_AVX
            __m256      a = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 result = AVX_Multiply(a, (1.0f / magnitude()));
            
            return setVector(result);

#elif defined(USE_SSE)
            __m128      a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 result = SSE_Multiply(a, (1.0f / magnitude()));
            
            return setVector(result);
#else
            float mag = magnitude();
            return Vector(x / mag, y / mag);
#endif
        }

        float dot(const Vector& operand) const noexcept {
#ifdef USE_AVX
            __m256 a = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 b = AVX_Set(operand.x, operand.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            
            return AVX_Dot(a, b);

#elif defined(USE_SSE)
            __m128 a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 b = SSE_Set(operand.x, operand.y, 0.0f, 0.0f);
           
            return SSE_Dot(a, b);

#else
            return x * operand.x + y * operand.y;
#endif
        }

        float magnitude() const noexcept {
            return std::sqrt(magnitudeSquared());
        }

        float magnitudeSquared() const noexcept {
            return dot(*this);
        }

        bool isCollinear(const Vector& operand, float epsilon = 1e-5f) const noexcept {
            float dotProduct = dot(operand);
            return std::abs(
                dotProduct 
                - (magnitude() * operand.magnitude())) 
                < epsilon;
        }

        bool isCollinearOpposite(const Vector& operand, float epsilon = 1e-5f) const noexcept {
            float dotProduct = dot(operand);
            return std::abs(
                dotProduct 
                + (magnitude() * operand.magnitude())) 
                < epsilon;
        }

        bool isPerpendicular(const Vector& operand, float epsilon = 1e-5f) const noexcept {
            return std::abs(dot(operand)) < epsilon;
        }

        bool isSameDirection(const Vector& operand) const noexcept {
            return dot(operand) > 0;
        }

        bool isOppositeDirection(const Vector& operand) const noexcept {
            return dot(operand) < 0;
        }

        /**********************
        *      utilities      *
        **********************/

        Vector setVector(__m256 result) const noexcept {
            return Vector(AVX_GetX(result), AVX_GetY(result));
        }

        Vector setVector(__m128 result) const noexcept {
            return Vector(SSE_GetX(result), SSE_GetY(result));
        }

        std::string ToString() const noexcept {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }
        };

        template <typename T>
        struct Vector<T, 3> {
            T x, y, z;

            /**********************
            *    constructors     *
            **********************/

            constexpr Vector() noexcept 
                : x(T()), y(T()), z(T()) {}

            Vector(T px, T py, T pz) noexcept 
                : x(px), y(py), z(pz) {}

            /**********************
            *  operator overloads *
            **********************/

            Vector operator+(const Vector& operand) const noexcept {
#ifdef USE_AVX
                __m256      a = AVX_Set(x, y, z, T(), T(), T(), T(), T());
                __m256      b = AVX_Set(operand.x, operand.y, operand.z, T(), T(), T(), T(), T());
                __m256 result = AVX_Add(a, b);

                return setVector(result);

#elif defined(USE_SSE)
                __m128      a = SSE_Set(x, y, z, T());
                __m128      b = SSE_Set(operand.x, operand.y, operand.z, T());
                __m128 result = SSE_Add(a, b);

                return setVector(result);
#else
                return Vector(
                    x + operand.x, 
                    y + operand.y, 
                    z + operand.z);
#endif
            }

            Vector operator-(const Vector& operand) const noexcept {
#ifdef USE_AVX
                __m256      a = AVX_Set(x, y, z, T(), T(), T(), T(), T());
                __m256      b = AVX_Set(operand.x, operand.y, operand.z, T(), T(), T(), T(), T());
                __m256 result = AVX_Subtract(a, b);

                return setVector(result);

#elif defined(USE_SSE)
                __m128      a = SSE_Set(x, y, z, T());
                __m128      b = SSE_Set(operand.x, operand.y, operand.z, T());
                __m128 result = SSE_Subtract(a, b);

                return setVector(result);
#else
                return Vector(
                    x - operand.x, 
                    y - operand.y, 
                    z - operand.z);
#endif
            }

            Vector operator*(T scalar) const noexcept {
#ifdef USE_AVX
                __m256      a = AVX_Set(x, y, z);
                __m256 result = AVX_Multiply(a, scalar);

                return setVector(result);

#elif defined(USE_SSE)
                __m128      a = SSE_Set(x, y, z);
                __m128 result = SSE_Multiply(a, scalar);

                return setVector(result);
#else
                return Vector(
                    x * scalar, 
                    y * scalar, 
                    z * scalar);
#endif
            }

            bool operator==(const Vector<float, 3>& other) const noexcept {
#ifdef USE_AVX
                __m256 a = AVX_Set(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                __m256 b = AVX_Set(other.x, other.y, other.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                return AVX_AllTrue(AVX_CompareEqual(a, b));
#elif defined(USE_SSE)
                __m128 a = SSE_Set(x, y, z, 0.0f);
                __m128 b = SSE_Set(other.x, other.y, other.z, 0.0f);
                return SSE_AllTrue(SSE_CompareEqual(a, b));
#else
                return x == other.x && y == other.y && z == other.z;
#endif
            }

            // Inequality operator
            bool operator!=(const Vector<float, 3>& other) const noexcept {
                return !(*this == other);
            }

            /**********************
            *       methods       *
            **********************/

            T dot(const Vector& operand) const noexcept {
#ifdef USE_AVX
                __m256 a = AVX_Set(x, y, z);
                __m256 b = AVX_Set(
                                  operand.x, 
                                  operand.y, 
                                  operand.z);
                return AVX_Dot(a, b);
#elif defined(USE_SSE)
                __m128 a = SSE_Set(x, y, z);
                __m128 b = SSE_Set(
                                  operand.x, 
                                  operand.y, 
                                  operand.z);
                return SSE_Dot(a, b);
#else
                return x * operand.x 
                     + y * operand.y 
                     + z * operand.z;
#endif
            }

            T magnitude() const noexcept {
                return std::sqrt(magnitudeSquared());
            }

            T magnitudeSquared() const noexcept {
                return dot(*this);
            }

            Vector unitVector() const noexcept {
#ifdef USE_AVX
                __m256      a = AVX_Set(x, y, z);
                __m256 result = AVX_Multiply(a, (1.0f / magnitude()));
                
                return setVector(result);

#elif defined(USE_SSE)
                __m128      a = SSE_Set(x, y, z);
                __m128 result = SSE_Multiply(a, (1.0f / magnitude()));

                return setVector(result);
#else
                T mag = magnitude();
                return Vector(
                              x / mag,
                              y / mag, 
                              z / mag);
#endif
            }

            Vector cross(const Vector& operand) const noexcept {
#ifdef USE_AVX
                __m256 a = AVX_Set(x, y, z);
                __m256 b = AVX_Set(
                                   operand.x,
                                   operand.y,
                                   operand.z);

                // shuffle components for cross product computation
                __m256 a_yzx = AVX_ShuffleYZXW(a);
                __m256 b_yzx = AVX_ShuffleYZXW(b);
                __m256 a_zxy = AVX_ShuffleZXYW(a);
                __m256 b_zxy = AVX_ShuffleZXYW(b);

                __m256 cross = AVX_Subtract(AVX_Multiply(a_yzx, b_zxy), AVX_Multiply(a_zxy, b_yzx));
                
                return setVector(cross);

#elif defined(USE_SSE)
                __m128 a = SSE_Set(x, y, z, 0.0f);
                __m128 b = SSE_Set(operand.x, operand.y, operand.z, 0.0f);

                // shuffle components for cross product computation
                __m128 a_yzx = SSE_ShuffleYZXW(a);
                __m128 b_yzx = SSE_ShuffleYZXW(b);
                __m128 a_zxy = SSE_ShuffleZXYW(a);
                __m128 b_zxy = SSE_ShuffleZXYW(b);

                __m128 cross = SSE_Subtract(
                    SSE_Multiply(a_yzx, b_zxy), 
                    SSE_Multiply(a_zxy, b_yzx));
                
                return setVector(cross);
#else
                return Vector(
                    (y * operand.z) - (z * operand.y),
                    (z * operand.x) - (x * operand.z),
                    (x * operand.y) - (y * operand.x)
                );
#endif
            }


            /**********************
            *      utilities      *
            **********************/

            Vector setVector(__m256 result) const noexcept {
                return Vector(AVX_GetX(result), AVX_GetY(result), AVX_GetZ(result));
            }

            Vector setVector(__m128 result) const noexcept {
                return Vector(SSE_GetX(result), SSE_GetY(result), SSE_GetZ(result));
            }

            std::string ToString() const noexcept {
                return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
            }
        };

}
