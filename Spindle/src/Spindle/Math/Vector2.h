#pragma once

#include "../Core.h"
#include "SSE.h"
#include "../Debug/Tools/StopWatch.h"

#include <string>

namespace Spindle {

    // all other datatypes use regular math
    template <typename T>
    struct alignas(16) Vector2 {
    public:
        T x, y;

        constexpr Vector2() noexcept
            : x{ T() }, y{ T() } {}

        Vector2(float p_x, float p_y) noexcept
            : x{ p_x }, y{ p_y } {}

        // addition
        constexpr Vector2 operator+(const Vector2& operand) const noexcept {
            return Vector2(x + operand.x, y + operand.y);
        }

        // subtraction
        constexpr Vector2 operator-(const Vector2& operand) const noexcept {
            return Vector2(x - operand.x, y - operand.y);
        }

        // multiplication by a scalar
        constexpr Vector2 operator*(T scalar) const noexcept {
            return Vector2(x * scalar, y * scalar);
        }

        // getters for x and y
        T getX() const noexcept { return x; }
        T getY() const noexcept { return y; }
    };

    // floats use SIMD
    template <>
    struct alignas(16) Vector2<float> {
    private:
        float x, y;

    public:
        Vector2() noexcept
            : x{ 0.0f }, y{ 0.0f } {}

        Vector2(float p_x, float p_y) noexcept
            : x{ p_x }, y{ p_y } {}

        // addition (SIMD)
        Vector2 operator+(const Vector2<float>& operand) const noexcept {
            __m128 a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 b = SSE_Set(operand.x, operand.y, 0.0f, 0.0f);

            __m128 result = SSE_Add(a, b);
            return Vector2(SSE_GetX(result), SSE_GetY(result));
        }

        // subtraction (SIMD)
        Vector2 operator-(const Vector2<float>& operand) const noexcept {
            __m128 a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 b = SSE_Set(operand.x, operand.y, 0.0f, 0.0f);

            __m128 result = SSE_Subtract(a, b);
            return Vector2(SSE_GetX(result), SSE_GetY(result));
        }

        // multiplication by a scalar (SIMD)
        Vector2 operator*(float scalar) const noexcept {
            __m128 a = SSE_Set(x, y, 0.0f, 0.0f);

            __m128 result = SSE_MultiplyScalar(a, scalar);
            return Vector2(SSE_GetX(result), SSE_GetY(result));
        }

        // dot product TODO stopwatch this. is it slow? horizontal adding n that
        float dot(const Vector2<float>& operand) noexcept {
            // load the vectors into __m128 registers
            __m128 a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 b = SSE_Set(operand.x, operand.y, 0.0f, 0.0f);

            return SSE_Dot(a, b);
        }

        std::string toString() const {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }

        // getters for x and y
        float getX() const noexcept { return x; }
        float getY() const noexcept { return y; }
    };
}
