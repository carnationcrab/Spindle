#pragma once
#include "SSE/SSE.h"
#include "AVX/AVX.h"
#include "Vector.h"

#include <cmath>
#include <string>

/**************************
*                         *
* point (x, y, z)         *
*                         *
**************************/

namespace Spindle {
    // all other types use scalar
    template <typename T, size_t Dimension>
    struct Point {
        T coordinates[Dimension];

        /**********************
        *    constructors     *
        **********************/

        constexpr Point() noexcept {
            for (size_t i = 0; i < Dimension; ++i) {
                coordinates[i] = T();
            }
        }

        Point(const T(&coords)[Dimension]) {
            for (size_t i = 0; i < Dimension; ++i) {
                coordinates[i] = coords[i];
            }
        }

        /**********************
        *  operator overloads *
        **********************/

        Point operator+(const Vector<T, Dimension>& vec) const {
            T result[Dimension];
            for (size_t i = 0; i < Dimension; ++i) {
                result[i] = coordinates[i] + vec[i];
            }
            return Point(result);
        }

        Vector<T, Dimension> operator-(const Point& other) const {
            T result[Dimension];
            for (size_t i = 0; i < Dimension; ++i) {
                result[i] = coordinates[i] - other.coordinates[i];
            }
            return Vector<T, Dimension>(result);
        }

        bool operator==(const Point& other) const {
            for (size_t i = 0; i < Dimension; ++i) {
                if (coordinates[i] != other.coordinates[i]) return false;
            }
            return true;
        }

        bool operator!=(const Point& other) const {
            return !(*this == other);
        }

        /**********************
        *       methods       *
        **********************/

        T distanceTo(const Point& other) const {
            T sum = T();
            for (size_t i = 0; i < Dimension; ++i) {
                T diff = coordinates[i] - other.coordinates[i];
                sum += diff * diff;
            }
            return std::sqrt(sum);
        }

        T distanceSquaredTo(const Point& other) const {
            T sum = T();
            for (size_t i = 0; i < Dimension; ++i) {
                T diff = coordinates[i] - other.coordinates[i];
                sum += diff * diff;
            }
            return sum;
        }

        Point lerp(const Point& other, T t) const {
            T result[Dimension];
            for (size_t i = 0; i < Dimension; ++i) {
                result[i] = coordinates[i] + (other.coordinates[i] - coordinates[i]) * t;
            }
            return Point(result);
        }

        std::string toString() const {
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
    struct Point<float, 2> {
        alignas(16) float x, y;

        /**********************
        *    constructors     *
        **********************/

        constexpr Point() noexcept : x(0.0f), y(0.0f) {}

        Point(float px, float py) noexcept : x(px), y(py) {}

        /**********************
        *  operator overloads *
        **********************/

        Point operator+(const Vector<float, 2>& vec) const noexcept {
#ifdef USE_AVX
            __m256 p = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 v = AVX_Set(vec.x, vec.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 result = AVX_Add(p, v);
            return Point(AVX_GetX(result), AVX_GetY(result));
#elif defined(USE_SSE)
            __m128 p = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 v = SSE_Set(vec.x, vec.y, 0.0f, 0.0f);
            __m128 result = SSE_Add(p, v);
            return Point(SSE_GetX(result), SSE_GetY(result));
#else
            return Point(x + vec.x, y + vec.y);
#endif
        }

        Vector<float, 2> operator-(const Point& other) const noexcept {
#ifdef USE_AVX
            __m256 p1 = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 p2 = AVX_Set(other.x, other.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 result = AVX_Subtract(p1, p2);
            return Vector<float, 2>(AVX_GetX(result), AVX_GetY(result));
#elif defined(USE_SSE)
            __m128 p1 = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 p2 = SSE_Set(other.x, other.y, 0.0f, 0.0f);
            __m128 result = SSE_Subtract(p1, p2);
            return Vector<float, 2>(SSE_GetX(result), SSE_GetY(result));
#else
            return Vector<float, 2>(x - other.x, y - other.y);
#endif
        }


        bool operator==(const Point& other) const noexcept {
#ifdef USE_AVX
            __m256 p1 = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 p2 = AVX_Set(other.x, other.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 cmp = AVX_CompareEqual(p1, p2);
            return AVX_AllTrue(cmp); // Checks if all components are equal
#elif defined(USE_SSE)
            __m128 p1 = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 p2 = SSE_Set(other.x, other.y, 0.0f, 0.0f);
            __m128 cmp = SSE_CompareEqual(p1, p2);
            return SSE_AllTrue(cmp); // Checks if all components are equal
#else
            return x == other.x && y == other.y;
#endif
        }


        bool operator!=(const Point& other) const noexcept {
#ifdef USE_AVX
            __m256 p1 = AVX_Set(x, y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 p2 = AVX_Set(other.x, other.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 cmp = AVX_CompareNotEqual(p1, p2);
            return AVX_AnyTrue(cmp); // Checks if any component is not equal
#elif defined(USE_SSE)
            __m128 p1 = SSE_Set(x, y, 0.0f, 0.0f);
            __m128 p2 = SSE_Set(other.x, other.y, 0.0f, 0.0f);
            __m128 cmp = SSE_CompareNotEqual(p1, p2);
            return SSE_AnyTrue(cmp); // Checks if any component is not equal
#else
            return !(*this == other);
#endif
        }


        /**********************
        *       methods       *
        **********************/

        float distanceTo(const Point& other) const noexcept {
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

        float distanceSquaredTo(const Point& other) const noexcept {
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

        Point lerp(const Point& other, float t) const noexcept {
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
            float nx = x + (other.x - x) * t;
            float ny = y + (other.y - y) * t;
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

    template <>
    struct Point<float, 3> {
        alignas(16) float x, y, z;

        /**********************
        *    constructors     *
        **********************/

        constexpr Point() noexcept : x(0.0f), y(0.0f), z(0.0f) {}

        Point(float px, float py, float pz) noexcept : x(px), y(py), z(pz) {}

        /**********************
        *  operator overloads *
        **********************/

        Point operator+(const Vector<float, 3>& vec) const noexcept {
#ifdef USE_AVX
            __m256 p = AVX_Set(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 v = AVX_Set(vec.x, vec.y, vec.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 result = AVX_Add(p, v);
            return Point(AVX_GetX(result), AVX_GetY(result), AVX_GetZ(result));
#elif defined(USE_SSE)
            __m128 p = SSE_Set(x, y, z, 0.0f);
            __m128 v = SSE_Set(vec.x, vec.y, vec.z, 0.0f);
            __m128 result = SSE_Add(p, v);
            return Point(SSE_GetX(result), SSE_GetY(result), SSE_GetZ(result));
#else
            return Point(x + vec.x, y + vec.y, z + vec.z);
#endif
        }


        Vector<float, 3> operator-(const Point& other) const noexcept {
#ifdef USE_AVX
            __m256 p1 = AVX_Set(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 p2 = AVX_Set(other.x, other.y, other.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 result = AVX_Subtract(p1, p2);
            return Vector<float, 3>(AVX_GetX(result), AVX_GetY(result), AVX_GetZ(result));
#elif defined(USE_SSE)
            __m128 p1 = SSE_Set(x, y, z, 0.0f);
            __m128 p2 = SSE_Set(other.x, other.y, other.z, 0.0f);
            __m128 result = SSE_Subtract(p1, p2);
            return Vector<float, 3>(SSE_GetX(result), SSE_GetY(result), SSE_GetZ(result));
#else
            return Vector<float, 3>(x - other.x, y - other.y, z - other.z);
#endif
        }


        bool operator==(const Point& other) const noexcept {
#ifdef USE_AVX
            __m256 p1 = AVX_Set(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 p2 = AVX_Set(other.x, other.y, other.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 cmp = AVX_CompareEqual(p1, p2);
            return AVX_AllTrue(cmp);
#elif defined(USE_SSE)
            __m128 p1 = SSE_Set(x, y, z, 0.0f);
            __m128 p2 = SSE_Set(other.x, other.y, other.z, 0.0f);
            __m128 cmp = SSE_CompareEqual(p1, p2);
            return SSE_AllTrue(cmp);
#else
            return x == other.x && y == other.y && z == other.z;
#endif
        }

        bool operator!=(const Point& other) const noexcept {
#ifdef USE_AVX
            __m256 p1 = AVX_Set(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 p2 = AVX_Set(other.x, other.y, other.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 cmp = AVX_CompareNotEqual(p1, p2);
            return AVX_AnyTrue(cmp);
#elif defined(USE_SSE)
            __m128 p1 = SSE_Set(x, y, z, 0.0f);
            __m128 p2 = SSE_Set(other.x, other.y, other.z, 0.0f);
            __m128 cmp = SSE_CompareNotEqual(p1, p2);
            return SSE_AnyTrue(cmp);
#else
            return !(*this == other);
#endif
        }


        /**********************
        *       methods       *
        **********************/

        float distanceTo(const Point& other) const noexcept {
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
            float dx = x - other.x;
            float dy = y - other.y;
            float dz = z - other.z;
            return std::sqrt(dx * dx + dy * dy + dz * dz);
#endif
        }

        float distanceSquaredTo(const Point& other) const noexcept {
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
            float dx = x - other.x;
            float dy = y - other.y;
            float dz = z - other.z;
            return dx * dx + dy * dy + dz * dz;
#endif
        }

        Point lerp(const Point& other, float t) const noexcept {
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
            float nx = x + (other.x - x) * t;
            float ny = y + (other.y - y) * t;
            float nz = z + (other.z - z) * t;
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
