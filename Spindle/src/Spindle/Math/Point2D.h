#pragma once
#include "SSE/SSE.h"
#include "AVX/AVX.h"
#include "Vector2.h"

#include <cmath>
#include <string>

/**************************
*                         *
* Point (2D)   (2.0,1.0)  *
*                         *
**************************/

namespace Spindle {
    struct Point2D {
        float x, y;

        /**********************
        *                     *
        *    constructors     *
        *                     *
        **********************/

        constexpr Point2D() noexcept
            : x{ 0.0f }, y{ 0.0f } {}

        Point2D(float p_x, float p_y)
            : x{ p_x }, y{ p_y } {}

        /**********************
        *                     *
        * operator overloads  *
        *                     *
        **********************/

        Point2D operator+(const Vector2<float>& vec) const {
            return Point2D(x + vec.getX(), y + vec.getY());
        }

        Vector2<float> operator-(const Point2D& other) const {
            return Vector2<float>(x - other.x, y - other.y);
        }

        // equality checks
        bool operator==(const Point2D& other) const {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Point2D& other) const {
            return !(*this == other);
        }

        /**********************
        *                     *
        *       methods       *
        *                     *
        **********************/

        // height (simple math todo upgrade)
        float height(const Point2D& other) const {
            return other.y - y;
        }

        // distance between two points
        float distanceBetween(const Point2D& other) const {
#ifdef USE_AVX
            __m256 p1 = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 p2 = AVX_Set(other.x, other.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 diff = AVX_Subtract(p1, p2);
            return std::sqrt(AVX_Dot(diff, diff));
#elif defined(USE_SSE)
            __m128 p1 = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 p2 = SSE_Set(other.x, other.y, 0.0f, 0.0f);
            __m128 diff = SSE_Subtract(p1, p2);
            return std::sqrt(SSE_Dot(diff, diff));
#else
            float dx = x - other.x;
            float dy = y - other.y;
            return std::sqrt(dx * dx + dy * dy);
#endif
        }

        // squared
        float distanceSquaredBetween(const Point2D& other) const {
#ifdef USE_AVX
            __m256 p1 = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 p2 = AVX_Set(other.x, other.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 diff = AVX_Subtract(p1, p2);
            return AVX_Dot(diff, diff);
#elif defined(USE_SSE)
            __m128 p1 = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 p2 = SSE_Set(other.x, other.y, 0.0f, 0.0f);
            __m128 diff = SSE_Subtract(p1, p2);
            return SSE_Dot(diff, diff);
#else
            float dx = x - other.x;
            float dy = y - other.y;
            return dx * dx + dy * dy;
#endif
        }

        // linear interpolate
        Point2D lerp(const Point2D& other, float t) const {
#ifdef USE_AVX
            __m256 p1 = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 p2 = AVX_Set(other.x, other.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 tVec = AVX_Set(t);
            __m256 result = AVX_Add(p1, AVX_Multiply(AVX_Subtract(p2, p1), tVec));
            return setPoint2D(result);
#elif defined(USE_SSE)
            __m128 p1 = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 p2 = SSE_Set(other.x, other.y, 0.0f, 0.0f);
            __m128 tVec = _mm_set1_ps(t);
            __m128 result = SSE_Add(p1, SSE_Multiply(SSE_Subtract(p2, p1), tVec));
            return setPoint2D(result);
#else
            float nx = x + (other.x - x) * t;
            float ny = y + (other.y - y) * t;
            return Point2D(nx, ny);
#endif
        }

        /**********************
        *                     *
        *      utilities      *
        *                     *
        **********************/

        Point2D setPoint2D(__m256 result) const noexcept {
            return Point2D(AVX_GetX(result), AVX_GetY(result));
        }

        Point2D setPoint2D(__m128 result) const noexcept {
            return Point2D(SSE_GetX(result), SSE_GetY(result));
        }

        std::string toString() const {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }
    };
}