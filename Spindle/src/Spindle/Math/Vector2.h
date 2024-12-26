#pragma once

#include "../Core.h"
#include "SSE/SSE.h"
#include "AVX/AVX.h"

#include <cmath>
#include <string>

/**************************
*                         *
* Vector (2D)  (2.0,1.0)  *
*                         *
**************************/

namespace Spindle {

    // all other datatypes use regular math
    template <typename T>
    struct alignas(16) Vector2 {
    private:
        float x, y;

    public:

        constexpr Vector2() noexcept
            : x{ T() }, 
              y{ T() } {}

        Vector2(float p_x, float p_y) noexcept
            : x{ p_x }, 
              y{ p_y } {}

        // addition
        constexpr  Vector2 operator+(const Vector2& operand) const noexcept {
            return Vector2(
                x + operand.x, 
                y + operand.y
            );
        }

        // subtraction
        constexpr  Vector2 operator-(const Vector2& operand) const noexcept {
            return Vector2(
                x - operand.x, 
                y - operand.y
            );
        }

        // multiplication by a scalar
        constexpr  Vector2 operator*(T scalar) const noexcept {
            return Vector2(
                x * scalar, 
                y * scalar);
        }

        // collinear test (same direction)
        bool isCollinear(const Vector2& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);
            float       magA = magnitude();
            float       magB = operand.magnitude();

            return std::abs(dotProduct - (magA * magB)) < epsilon;
        }

        // collinear but opposite
        bool isCollinearOpposite(const Vector2& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);
            float       magA = magnitude();
            float       magB = operand.magnitude();

            return std::abs(dotProduct + (magA * magB)) < epsilon;
        }

        // perpendicular test
        bool isPerpendicular(const Vector2& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);

            return std::abs(dotProduct) < epsilon;
        }

        // same direction
        bool isSameDirection(const Vector2& operand) const noexcept {
            return dot(operand) > 0;
        }

        // opposite direction
        bool isOppositeDirection(const Vector2& operand) const noexcept {
            return dot(operand) < 0;
        }

        // string output
        std::string toString() const {
            return "(" + std::to_string(x) + ", "
                       + std::to_string(y) + ")";
        }

         // getters/setters
           T getX()     const noexcept { return x; }
        void setX(T newX) noexcept { x = newX; }

           T getY()     const noexcept { return y; }
        void setY(T newY) noexcept { y = newY; }
    };

    // floats use SIMD
    template <>
    struct alignas(16) Vector2<float> {
    private:
        float x, y;

        /**********************
        *                     *
        *    constructors     *
        *                     *
        **********************/

    public:
        Vector2() noexcept
            : x{ 0.0f }, y{ 0.0f } {}

        Vector2(float p_x, float p_y) noexcept
            : x{ p_x }, y{ p_y } {}

        /**********************
        *                     *
        * operator overloads  *
        *                     *
        **********************/

        // addition
        Vector2 operator+(const Vector2<float>& operand) const noexcept {
#ifdef USE_AVX
            __m256 a = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 b = AVX_Set(operand.x, operand.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 result = AVX_Add(a, b);
            return setVector2(result);
#elif defined(USE_SSE)
            __m128 a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 b = SSE_Set(operand.x, operand.y, 0.0f, 0.0f);
            __m128 result = SSE_Add(a, b);
            return setVector2(result);
#else
            return Vector2(x + operand.x, y + operand.y);
#endif
        }

        // subtraction
        Vector2 operator-(const Vector2<float>& operand) const noexcept {
#ifdef USE_AVX
            __m256 a = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 b = AVX_Set(operand.x, operand.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 result = AVX_Subtract(a, b);
            return setVector2(result);
#elif defined(USE_SSE)
            __m128 a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 b = SSE_Set(operand.x, operand.y, 0.0f, 0.0f);
            __m128 result = SSE_Subtract(a, b);
            return setVector2(result);
#else
            return Vector2(x - operand.x, y - operand.y);
#endif
        }

        // multiplication by a scalar
        Vector2 operator*(float scalar) const noexcept {
#ifdef USE_AVX
            __m256 a = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 result = AVX_Multiply(a, scalar);
            return setVector2(result);
#elif defined(USE_SSE)
            __m128 a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 result = SSE_Multiply(a, scalar);
            return setVector2(result);
#else
            return Vector2(x * scalar, y * scalar);
#endif
        }

        /**********************
        *                     *
        *       methods       *
        *                     *
        **********************/

        // normalized vector, unit vector
        Vector2 unitVector() const noexcept {
#ifdef USE_AVX
            __m256 a = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 result = AVX_Multiply(a, (1.0f / magnitude()));
            return setVector2(result);
#elif defined(USE_SSE)
            __m128 a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 result = SSE_Multiply(a, (1.0f / magnitude()));
            return setVector2(result);
#else
            float mag = magnitude();
            return Vector2(x / mag, y / mag);
#endif
        }

        // dot product
        float dot(const Vector2<float>& operand) const noexcept {
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

        // magnitude squared (length squared)
        float magnitudeSquared() const noexcept {
            return dot(*this);
        }

        // magnitude (length)
        float magnitude() const noexcept {
            return std::sqrt(magnitudeSquared());
        }

        // collinear test
        bool isCollinear(const Vector2& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);
            float magA = magnitude();
            float magB = operand.magnitude();
            return std::abs(dotProduct - (magA * magB)) < epsilon;
        }

        // collinear but opposite
        bool isCollinearOpposite(const Vector2& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);
            float magA = magnitude();
            float magB = operand.magnitude();
            return std::abs(dotProduct + (magA * magB)) < epsilon;
        }

        // perpendicular test
        bool isPerpendicular(const Vector2& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);
            return std::abs(dotProduct) < epsilon;
        }

        // same direction
        bool isSameDirection(const Vector2& operand) const noexcept {
            return dot(operand) > 0;
        }

        // opposite direction
        bool isOppositeDirection(const Vector2& operand) const noexcept {
            return dot(operand) < 0;
        }

        /**********************
        *                     *
        *      utilities      *
        *                     *
        **********************/

        Vector2 setVector2(__m256 result) const noexcept {
            return Vector2(AVX_GetX(result), AVX_GetY(result));
        }

        Vector2 setVector2(__m128 result) const noexcept {
            return Vector2(SSE_GetX(result), SSE_GetY(result));
        }

        std::string toString() const noexcept {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }

        // getters/setters
        float getX() const noexcept { return x; }
        void setX(float newX) noexcept { x = newX; }

        float getY() const noexcept { return y; }
        void setY(float newY) noexcept { y = newY; }
    };
}