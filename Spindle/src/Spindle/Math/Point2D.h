#pragma once
#include "Vector2.h"
#include "SSE.h"
#include <cmath>
#include <string>

namespace Spindle {
    struct Point2D {
        float x, y;

        // Constructors
        constexpr Point2D() noexcept
            : x{ 0.0f },
            y{ 0.0f } {}

        Point2D(float p_x, float p_y)
            : x{ p_x },
            y{ p_y } {}

        // Adding a vector results in a new point
        Point2D operator+(const Vector2<float>& vec) const {
            return Point2D(x + vec.getX(), y + vec.getY());
        }

        // Subtracting another point yields a vector
        Vector2<float> operator-(const Point2D& other) const {
            return Vector2<float>(x - other.x, y - other.y);
        }

        // Linear interpolation
        Point2D lerp(const Point2D& other, float t) const {
            __m128 p1 = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 p2 = SSE_Set(other.x, other.y, 0.0f, 0.0f);
            __m128 tVec = _mm_set1_ps(t);
            __m128 result = SSE_Add(p1, SSE_Multiply(SSE_Subtract(p2, p1), tVec));

            return Point2D(SSE_GetX(result), SSE_GetY(result));
        }

        // Equality checks
        bool operator==(const Point2D& other) const {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Point2D& other) const {
            return !(*this == other);
        }

        // Height above a line (simplified for now)
        float height(const Point2D& other) const {
            return other.y - y;
        }

        // Distance between two points
        float distanceBetween(const Point2D& other) const {
            __m128 p1 = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 p2 = SSE_Set(other.x, other.y, 0.0f, 0.0f);
            __m128 diff = SSE_Subtract(p1, p2);
            return std::sqrt(SSE_Dot(diff, diff));
        }

        float distanceSquaredBetween(const Point2D& other) const {
            __m128 p1 = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 p2 = SSE_Set(other.x, other.y, 0.0f, 0.0f);
            __m128 diff = SSE_Subtract(p1, p2);
            return SSE_Dot(diff, diff);
        }

        // String representation
        std::string toString() const {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }
    };
}
