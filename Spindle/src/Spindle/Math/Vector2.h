#pragma once

#include "../Core.h"
#include "SSE.h"

#include <cmath>
#include <string>

namespace Spindle {

    // all other datatypes use regular math
    template <typename T>
    struct alignas(16) Vector2 {
    public:
        T x, y;

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

        // Collinear test (same direction)
        bool isCollinear(const Vector2& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);
            float       magA = magnitude();
            float       magB = operand.magnitude();

            return std::abs(dotProduct - (magA * magB)) < epsilon;
        }

        // Collinear but opposite
        bool isCollinearOpposite(const Vector2& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);
            float       magA = magnitude();
            float       magB = operand.magnitude();

            return std::abs(dotProduct + (magA * magB)) < epsilon;
        }

        // Perpendicular test
        bool isPerpendicular(const Vector2& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);

            return std::abs(dotProduct) < epsilon;
        }

        // Same direction
        bool isSameDirection(const Vector2& operand) const noexcept {
            return dot(operand) > 0;
        }

        // Opposite direction
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

    public:
        Vector2() noexcept
            : x{ 0.0f }, y{ 0.0f } {}

        Vector2(float p_x, float p_y) noexcept
            : x{ p_x }, y{ p_y } {}

        // addition (SIMD)
        Vector2 operator+(const Vector2<float>& operand) const noexcept {
            __m128      a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128      b = SSE_Set(operand.x, operand.y, 0.0f, 0.0f);
            __m128 result = SSE_Add(a, b);
            
            return Vector2(
                SSE_GetX(result), 
                SSE_GetY(result)
            );
        }

        // subtraction (SIMD)
        Vector2 operator-(const Vector2<float>& operand) const noexcept {
            __m128      a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128      b = SSE_Set(operand.x, operand.y, 0.0f, 0.0f);
            __m128 result = SSE_Subtract(a, b);
            
            return Vector2(
                SSE_GetX(result), 
                SSE_GetY(result)
            );
        }

        // multiplication by a scalar (SIMD)
        Vector2 operator*(float scalar) const noexcept {
            __m128      a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 result = SSE_MultiplyScalar(a, scalar);
            
            return Vector2(
                SSE_GetX(result), 
                SSE_GetY(result)
            );
        }

        // normalized vector, unit vector
        Vector2 unitVector() const noexcept {
            __m128      a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 result = SSE_MultiplyScalar(a, (1 / magnitude()));

            return Vector2(
                SSE_GetX(result),
                SSE_GetY(result)
            );
        }

        // dot product 
        // TODO stopwatch this. is it slow? 
        // horizontal adding n that
        float dot(const Vector2<float>& operand) const noexcept {
            __m128 a = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 b = SSE_Set(operand.x, operand.y, 0.0f, 0.0f);

            return SSE_Dot(a, b);
        }

        // magnitude squared 
        // (for comparing relative lengths)
        // more efficient, use this whenever possible 
        // instead of mag
        float magnitudeSquared() const noexcept {
            return dot(*this);
        }

        // magnitude (length)
        float magnitude() const noexcept {
            return std::sqrt(magnitudeSquared());
        }

        // Collinear test (same direction) constexpr?
        bool isCollinear(const Vector2& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);
            float       magA = magnitude();
            float       magB = operand.magnitude();

            return std::abs(dotProduct - (magA * magB)) < epsilon;
        }

        // Collinear but opposite
        bool isCollinearOpposite(const Vector2& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);
            float       magA = magnitude();
            float       magB = operand.magnitude();
            
            return std::abs(dotProduct + (magA * magB)) < epsilon;
        }

        // Perpendicular test
        bool isPerpendicular(const Vector2& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);

            return std::abs(dotProduct) < epsilon;
        }

        // Same direction
        bool isSameDirection(const Vector2& operand) const noexcept {
            return dot(operand) > 0;
        }

        // Opposite direction
        bool isOppositeDirection(const Vector2& operand) const noexcept {
            return dot(operand) < 0;
        }

        std::string toString() const noexcept {
            return "(" + std::to_string(x) + ", "
                + std::to_string(y) + ")";
        }

        // getters/setters
        float getX()     const noexcept { return x; }
         void setX(float newX) noexcept { x = newX; }

        float getY()     const noexcept { return y; }
         void setY(float newY) noexcept { y = newY; }
    };
}
