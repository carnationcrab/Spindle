#pragma once
#include "SSE/SSE.h"
#include "AVX/AVX.h"
#include "Vector.h"

#include <cmath>
#include <string>

/**************************
*                         *
* Point (Template)        *
*                         *
**************************/

namespace Spindle {
    template <typename T, size_t Dimension>
    struct Point;

    /********************************
    *                               *
    *    2D Point Specialization    *
    *                               *
    ********************************/

    template <typename T>
    struct Point<T, 2> {
        T x, y;

        /**********************
        *    Constructors     *
        **********************/

        constexpr Point() noexcept
            : x{ 0 }, y{ 0 } {}

        Point(T px, T py)
            : x{ px }, y{ py } {}

        /**********************
        *  Operator Overloads *
        **********************/

        Point operator+(const Vector<T, 2>& vec) const {
            return Point(x + vec.x, y + vec.y);
        }

        Vector<T, 2> operator-(const Point& other) const {
            return Vector<T, 2>(x - other.x, y - other.y);
        }

        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Point& other) const {
            return !(*this == other);
        }

        /**********************
        *       Methods       *
        **********************/

        T distanceTo(const Point& other) const {
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
            T dx = x - other.x;
            T dy = y - other.y;
            return std::sqrt(dx * dx + dy * dy);
#endif
        }

        T distanceSquaredTo(const Point& other) const {
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
            T dx = x - other.x;
            T dy = y - other.y;
            return dx * dx + dy * dy;
#endif
        }

        Point lerp(const Point& other, T t) const {
#ifdef USE_AVX
            __m256 p1 = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 p2 = AVX_Set(other.x, other.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 tVec = AVX_Set(t);
            __m256 result = AVX_Add(p1, AVX_Multiply(AVX_Subtract(p2, p1), tVec));
            return setPoint(result);
#elif defined(USE_SSE)
            __m128 p1 = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 p2 = SSE_Set(other.x, other.y, 0.0f, 0.0f);
            __m128 tVec = _mm_set1_ps(t);
            __m128 result = SSE_Add(p1, SSE_Multiply(SSE_Subtract(p2, p1), tVec));
            return setPoint(result);
#else
            T nx = x + (other.x - x) * t;
            T ny = y + (other.y - y) * t;
            return Point(nx, ny);
#endif
        }

        Point setPoint(__m256 result) const noexcept {
            return Point(AVX_GetX(result), AVX_GetY(result));
        }

        Point setPoint(__m128 result) const noexcept {
            return Point(SSE_GetX(result), SSE_GetY(result));
        }

        std::string toString() const {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }
    };

    /********************************
    *                               *
    *    3D Point Specialization    *
    *                               *
    ********************************/

    template <typename T>
    struct Point<T, 3> {
        T x, y, z;

        /**********************
        *    Constructors     *
        **********************/

        constexpr Point() noexcept
            : x{ 0 }, y{ 0 }, z{ 0 } {}

        Point(T px, T py, T pz)
            : x{ px }, y{ py }, z{ pz } {}

        /**********************
        *  Operator Overloads *
        **********************/

        Point operator+(const Vector<T, 3>& vec) const {
            return Point(x + vec.x, y + vec.y, z + vec.z);
        }

        Vector<T, 3> operator-(const Point& other) const {
            return Vector<T, 3>(x - other.x, y - other.y, z - other.z);
        }

        bool operator==(const Point& other) const {
            return x == other.x && y == other.y && z == other.z;
        }

        bool operator!=(const Point& other) const {
            return !(*this == other);
        }

        /**********************
        *       Methods       *
        **********************/

        T distanceTo(const Point& other) const {
#ifdef USE_AVX
            __m256 p1 = AVX_Set(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 p2 = AVX_Set(other.x, other.y, other.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 diff = AVX_Subtract(p1, p2);
            return std::sqrt(AVX_Dot(diff, diff));
#elif defined(USE_SSE)
            __m128 p1 = SSE_Set(x, y, z, 0.0f);
            __m128 p2 = SSE_Set(other.x, other.y, other.z, 0.0f);
            __m128 diff = SSE_Subtract(p1, p2);
            return std::sqrt(SSE_Dot(diff, diff));
#else
            T dx = x - other.x;
            T dy = y - other.y;
            T dz = z - other.z;
            return std::sqrt(dx * dx + dy * dy + dz * dz);
#endif
        }

        T distanceSquaredTo(const Point& other) const {
#ifdef USE_AVX
            __m256 p1 = AVX_Set(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 p2 = AVX_Set(other.x, other.y, other.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 diff = AVX_Subtract(p1, p2);
            return AVX_Dot(diff, diff);
#elif defined(USE_SSE)
            __m128 p1 = SSE_Set(x, y, z, 0.0f);
            __m128 p2 = SSE_Set(other.x, other.y, other.z, 0.0f);
            __m128 diff = SSE_Subtract(p1, p2);
            return SSE_Dot(diff, diff);
#else
            T dx = x - other.x;
            T dy = y - other.y;
            T dz = z - other.z;
            return dx * dx + dy * dy + dz * dz;
#endif
        }

        Point lerp(const Point& other, T t) const {
#ifdef USE_AVX
            __m256 p1 = AVX_Set(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 p2 = AVX_Set(other.x, other.y, other.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 tVec = AVX_Set(t);
            __m256 result = AVX_Add(p1, AVX_Multiply(AVX_Subtract(p2, p1), tVec));
            return setPoint(result);
#elif defined(USE_SSE)
            __m128 p1 = SSE_Set(x, y, z, 0.0f);
            __m128 p2 = SSE_Set(other.x, other.y, other.z, 0.0f);
            __m128 tVec = _mm_set1_ps(t);
            __m128 result = SSE_Add(p1, SSE_Multiply(SSE_Subtract(p2, p1), tVec));
            return setPoint(result);
#else
            T nx = x + (other.x - x) * t;
            T ny = y + (other.y - y) * t;
            T nz = z + (other.z - z) * t;
            return Point(nx, ny, nz);
#endif
        }

        Point setPoint(__m256 result) const noexcept {
            return Point(AVX_GetX(result), AVX_GetY(result), AVX_GetZ(result));
        }

        Point setPoint(__m128 result) const noexcept {
            return Point(SSE_GetX(result), SSE_GetY(result), SSE_GetZ(result));
        }

        std::string toString() const {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
        }
    };
}
