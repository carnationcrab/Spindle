#pragma once

#include "Point.h"
#include "Vector.h"

namespace Spindle {

    /********************************
    *                               *
    *     line (a to infinity)      *
    *                               *
    ********************************/

    template <typename T, size_t Dimension>
    struct Line {
         Point<T, Dimension> start;
        Vector<T, Dimension> direction;

        /**********************
        *    constructors     *
        **********************/

        constexpr Line() noexcept
            : start(), direction() {}

        Line(const Point<T, Dimension>& start, const Vector<T, Dimension>& dir)
            : start(start), direction(dir.unitVector()) {}

        /**********************
        *       methods       *
        **********************/

        // compute a point on the line for a given scalar parameter t
        Point<T, Dimension> getPoint(T t) const noexcept {
            return Point<T, Dimension>(
                start.x + t * direction.x,
                start.y + t * direction.y,
                (Dimension > 2 ? start.z + t * direction.z : T()));
        }

        // check if two lines are equal
        bool operator==(const Line& other) const noexcept {
            return start == other.start && direction == other.direction;
        }

        bool operator!=(const Line& other) const noexcept {
            return !(*this == other);
        }

        /**********************
        *      utilities      *
        **********************/

        std::string ToString() const {
            return "Line(start: " + start.ToString() + ", Direction: " + direction.ToString() + ")";
        }
    };

    // floats use SIMD
    template <size_t Dimension>
    struct  Line<float, Dimension> {
           Point<float, Dimension> start; // Starting point
          Vector<float, Dimension> direction; // Unit direction vector

        /**********************
        *    constructors     *
        **********************/

        constexpr Line() noexcept
            : start(), direction() {}

        Line(const Point<float, Dimension>& start, const Vector<float, Dimension>& dir)
            : start(start), direction(dir.unitVector()) {}

        /**********************
        *       methods       *
        **********************/

        // computes a point on the line for a given scalar parameter t
        Point<float, Dimension> getPoint(float t) const noexcept {
#ifdef USE_AVX
            __m256      p = AVX_Set(start.x, start.y, (Dimension > 2 ? start.z : 0.0f));
            __m256      d = AVX_Set(direction.x, direction.y, (Dimension > 2 ? direction.z : 0.0f));
            __m256   tVec = AVX_Set(t);
            __m256 result = AVX_Add(p, AVX_Multiply(d, tVec));

            return Point<float, Dimension>(AVX_GetX(result), AVX_GetY(result), (Dimension > 2 ? AVX_GetZ(result) : 0.0f));
#elif defined(USE_SSE)
            __m128      p = SSE_Set(start.x, start.y, (Dimension > 2 ? start.z : 0.0f));
            __m128      d = SSE_Set(direction.x, direction.y, (Dimension > 2 ? direction.z : 0.0f));
            __m128   tVec = _mm_set1_ps(t);
            __m128 result = SSE_Add(p, SSE_Multiply(d, tVec));

            return Point<float, Dimension>(SSE_GetX(result), SSE_GetY(result), (Dimension > 2 ? SSE_GetZ(result) : 0.0f));
#else
            return Point<float, Dimension>(
                start.x + t * direction.x,
                start.y + t * direction.y,
                (Dimension > 2 ? start.z + t * direction.z : 0.0f));
#endif
        }

        // checks if two lines are equal
        bool operator==(const Line& other) const noexcept {
#ifdef USE_AVX
            __m256 p1 = AVX_Set(start.x, start.y, (Dimension > 2 ? start.z : 0.0f));
            __m256 p2 = AVX_Set(other.start.x, other.start.y, (Dimension > 2 ? other.start.z : 0.0f));
            __m256 d1 = AVX_Set(direction.x, direction.y, (Dimension > 2 ? direction.z : 0.0f));
            __m256 d2 = AVX_Set(other.direction.x, other.direction.y, (Dimension > 2 ? other.direction.z : 0.0f));
           
            return AVX_AllTrue(AVX_CompareEqual(p1, p2)) && AVX_AllTrue(AVX_CompareEqual(d1, d2));

#elif defined(USE_SSE)
            __m128 p1 = SSE_Set(start.x, start.y, (Dimension > 2 ? start.z : 0.0f));
            __m128 p2 = SSE_Set(other.start.x, other.start.y, (Dimension > 2 ? other.start.z : 0.0f));
            __m128 d1 = SSE_Set(direction.x, direction.y, (Dimension > 2 ? direction.z : 0.0f));
            __m128 d2 = SSE_Set(other.direction.x, other.direction.y, (Dimension > 2 ? other.direction.z : 0.0f));
            
            return SSE_AllTrue(SSE_CompareEqual(p1, p2)) && SSE_AllTrue(SSE_CompareEqual(d1, d2));
#else
            return start == other.start && direction == other.direction;
#endif
        }

        bool operator!=(const Line& other) const noexcept {
            return !(*this == other);
        }

        /**********************
        *      utilities      *
        **********************/

        std::string ToString() const {
            return "Line(start: " + start.ToString() + ", Direction: " + direction.ToString() + ")";
        }
    };

}
