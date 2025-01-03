#pragma once

#include "Point.h"
#include "Vector.h"
#include <cmath>
#include <string>

namespace Spindle {

    /********************************
    *                               *
    *        line segment           *
    *                               *
    ********************************/

    template <typename T, size_t Dimension>
    struct LineSegment {
        Point<T, Dimension> start; // start point of the segment
        Point<T, Dimension> end;   // end point of the segment

        /**********************
        *    constructors     *
        **********************/

        constexpr LineSegment() noexcept
            : start(), end() {}

        LineSegment(const Point<T, Dimension>& p0, const Point<T, Dimension>& p1)
            : start(p0), end(p1) {}

        /**********************
        *       methods       *
        **********************/

        // computes a point on the segment for normalized t (0 <= t <= 1)
        Point<T, Dimension> getPoint(T t) const noexcept {

            assert(t < 0.0 || t > 1.0 && "parameter t must be in the range [0, 1] for a line segment.");

            // In production, clamp the value
            //t = std::max(T(0.0), std::min(t, 1.0));

            return start + (end - start) * t;
        }

        // computes a point on the segment for non-normalized t (0 <= t <= length)
        Point<T, Dimension> getPointNonNormalized(T t) const noexcept {
            T L = length();

            assert(t < 0.0 && t > L && "parameter t must be in the range [0, L] for a line segment.");

            // In production, clamp the value
            //t = std::max(T(0.0), std::min(t, L));

            Vector<T, Dimension> u = (end - start).unitVector();
            return start + u * t;
        }

        // length of the segment
        T length() const noexcept {
            return (end - start).magnitude();
        }

        // squared length of the segment
        T lengthSquared() const noexcept {
            return (end - start).magnitudeSquared();
        }

        // check if two line segments are equal
        bool operator==(const LineSegment& other) const noexcept {
            return start == other.start && end == other.end;
        }

        bool operator!=(const LineSegment& other) const noexcept {
            return !(*this == other);
        }

        /**********************
        *      utilities      *
        **********************/

        std::string toString() const {
            return "LineSegment(Start: " + start.toString() + ", End: " + end.toString() + ")";
        }
    };

    /********************************
    *                               *
    *   linesegment<float> specialization *
    *                               *
    ********************************/

    template <size_t Dimension>
    struct LineSegment<float, Dimension> {
        Point<float, Dimension> start; // start point of the segment
        Point<float, Dimension> end;   // end point of the segment

        /**********************
        *    constructors     *
        **********************/

        constexpr LineSegment() noexcept
            : start(), end() {}

        LineSegment(const Point<float, Dimension>& p0, const Point<float, Dimension>& p1)
            : start(p0), end(p1) {}

        /**********************
        *       methods       *
        **********************/

        // computes a point on the segment for normalized t (0 <= t <= 1)
        Point<float, Dimension> getPoint(float t) const noexcept {
            assert(t >= 0.0f && t <= 1.0f && "parameter t must be in the range [0, 1] for a line segment.");
            // In production, clamp the value
            //t = std::max(T(0.0), std::min(t, L));
            
#ifdef USE_AVX
            __m256 p0 = AVX_Set(start.x, start.y, (Dimension > 2 ? start.z : 0.0f));
            __m256 p1 = AVX_Set(end.x, end.y, (Dimension > 2 ? end.z : 0.0f));
            __m256 tVec = AVX_Set(t);
            __m256 result = AVX_Add(p0, AVX_Multiply(AVX_Subtract(p1, p0), tVec));

            return Point<float, Dimension>(AVX_GetX(result), AVX_GetY(result), (Dimension > 2 ? AVX_GetZ(result) : 0.0f));
#elif defined(USE_SSE)
            __m128 p0 = SSE_Set(start.x, start.y, (Dimension > 2 ? start.z : 0.0f), 0.0f);
            __m128 p1 = SSE_Set(end.x, end.y, (Dimension > 2 ? end.z : 0.0f), 0.0f);
            __m128 tVec = _mm_set1_ps(t);
            __m128 result = SSE_Add(p0, SSE_Multiply(SSE_Subtract(p1, p0), tVec));

            return Point<float, Dimension>(SSE_GetX(result), SSE_GetY(result), (Dimension > 2 ? SSE_GetZ(result) : 0.0f));
#else
            return start + (end - start) * t;
#endif
        }

        // computes a point on the segment for non-normalized t (0 <= t <= length)
        Point<float, Dimension> getPointNonNormalized(float t) const noexcept {
            float L = length();

            assert(t >= 0.0 && t <= L && "parameter t must be in the range [0, L] for a line segment.");

            // In production, clamp the value
            // t = std::max(T(0.0), std::min(t, L));

            Vector<float, Dimension> u = (end - start).unitVector();
            return start + u * t;
        }

        // length of the segment
        float length() const noexcept {
            return (end - start).magnitude();
        }

        // squared length of the segment
        float lengthSquared() const noexcept {
            return (end - start).magnitudeSquared();
        }

        // check if two line segments are equal
        bool operator==(const LineSegment& other) const noexcept {
#ifdef USE_AVX
            __m256 s1 = AVX_Set(start.x, start.y, (Dimension > 2 ? start.z : 0.0f));
            __m256 s2 = AVX_Set(other.start.x, other.start.y, (Dimension > 2 ? other.start.z : 0.0f));
            __m256 e1 = AVX_Set(end.x, end.y, (Dimension > 2 ? end.z : 0.0f));
            __m256 e2 = AVX_Set(other.end.x, other.end.y, (Dimension > 2 ? other.end.z : 0.0f));

            return AVX_AllTrue(AVX_CompareEqual(s1, s2)) && AVX_AllTrue(AVX_CompareEqual(e1, e2));
#elif defined(USE_SSE)
            __m128 s1 = SSE_Set(start.x, start.y, (Dimension > 2 ? start.z : 0.0f), 0.0f);
            __m128 s2 = SSE_Set(other.start.x, other.start.y, (Dimension > 2 ? other.start.z : 0.0f), 0.0f);
            __m128 e1 = SSE_Set(end.x, end.y, (Dimension > 2 ? end.z : 0.0f), 0.0f);
            __m128 e2 = SSE_Set(other.end.x, other.end.y, (Dimension > 2 ? other.end.z : 0.0f), 0.0f);

            return SSE_AllTrue(SSE_CompareEqual(s1, s2)) && SSE_AllTrue(SSE_CompareEqual(e1, e2));
#else
            return start == other.start && end == other.end;
#endif
        }

        bool operator!=(const LineSegment& other) const noexcept {
            return !(*this == other);
        }

        /**********************
        *      utilities      *
        **********************/

        std::string toString() const {
            return "LineSegment(Start: " + start.toString() + ", End: " + end.toString() + ")";
        }
    };

}
