#pragma once

#include "Vector.h"
#include "Point.h"
#include "AVX/AVX.h"
#include "SSE/SSE.h"
#include <cmath>
#include <string>
#include <immintrin.h>

namespace Spindle {

    template <typename T>
    class Plane {
    private:
        Vector<T, 3> normal; // normal vector of the plane
        T distance;          // signed distance from the plane to the origin

    public:
        // constructors
        constexpr Plane() noexcept : normal(), distance(0) {}
        Plane(const Vector<T, 3>& n, T d) : normal(n.unitVector()), distance(d) {}
        Plane(const Point<T, 3>& point, const Vector<T, 3>& n) : normal(n.unitVector()), distance(-(normal.dot(point))) {}

        // getters and setters
        const Vector<T, 3>& getNormal() const noexcept { return normal; }
        void setNormal(const Vector<T, 3>& n) noexcept { normal = n.unitVector(); }

        T getDistance() const noexcept { return distance; }
        void setDistance(T d) noexcept { distance = d; }

        // methods
        T signedDistance(const Point<T, 3>& point) const noexcept {
            return normal.dot(point) + distance;
        }

        bool contains(const Point<T, 3>& point) const noexcept {
            return std::abs(signedDistance(point)) < std::numeric_limits<T>::epsilon();
        }

        std::string toString() const {
            return "Plane(Normal: " + normal.toString() + ", Distance: " + std::to_string(distance) + ")";
        }
    };

    // specialization for float
    template <>
    class Plane<float> {
    private:
#ifdef USE_AVX
        __m256 normal; // packed [nx, ny, nz, 0]
        float distance;
#elif defined(USE_SSE)
        __m128 normal; // packed [nx, ny, nz, 0]
        float distance;
#else
        Vector<float, 3> normal;
        float distance;
#endif

    public:
        // constructors
        Plane() noexcept {
#ifdef USE_AVX
            normal = AVX_Set(0.0f, 0.0f, 0.0f, 0.0f);
            distance = 0.0f;
#elif defined(USE_SSE)
            normal = SSE_Set(0.0f, 0.0f, 0.0f, 0.0f);
            distance = 0.0f;
#else
            normal = Vector<float, 3>();
            distance = 0.0f;
#endif
        }

        Plane(const Vector<float, 3>& n, float d) {
#ifdef USE_AVX
            normal = AVX_Set(n.x, n.y, n.z, 0.0f);
            distance = d;
#elif defined(USE_SSE)
            normal = SSE_Set(n.x, n.y, n.z, 0.0f);
            distance = d;
#else
            normal = n.unitVector();
            distance = d;
#endif
        }

        Plane(const Point<float, 3>& point, const Vector<float, 3>& n) {
#ifdef USE_AVX
            normal = AVX_Set(n.x, n.y, n.z, 0.0f);
            distance = -AVX_Dot(normal, AVX_Set(point.x, point.y, point.z, 0.0f));
#elif defined(USE_SSE)
            normal = SSE_Set(n.x, n.y, n.z, 0.0f);
            distance = -SSE_Dot(normal, SSE_Set(point.x, point.y, point.z, 0.0f));
#else
            normal = n.unitVector();
            distance = -(normal.dot(point));
#endif
        }

        // getters and setters
        Vector<float, 3> getNormal() const noexcept {
#ifdef USE_AVX
            return Vector<float, 3>(
                AVX_GetX(normal),
                AVX_GetY(normal),
                AVX_GetZ(normal)
            );
#elif defined(USE_SSE)
            return Vector<float, 3>(
                SSE_GetX(normal),
                SSE_GetY(normal),
                SSE_GetZ(normal)
            );
#else
            return normal;
#endif
        }

        void setNormal(const Vector<float, 3>& n) noexcept {
#ifdef USE_AVX
            normal = AVX_Set(n.x, n.y, n.z, 0.0f);
#elif defined(USE_SSE)
            normal = SSE_Set(n.x, n.y, n.z, 0.0f);
#else
            normal = n.unitVector();
#endif
        }

        float getDistance() const noexcept { return distance; }
        void setDistance(float d) noexcept { distance = d; }

        // methods
        float signedDistance(const Point<float, 3>& point) const noexcept {
#ifdef USE_AVX
            __m256 p = AVX_Set(point.x, point.y, point.z, 0.0f);
            return AVX_Dot(normal, p) + distance;
#elif defined(USE_SSE)
            __m128 p = SSE_Set(point.x, point.y, point.z, 0.0f);
            return SSE_Dot(normal, p) + distance;
#else
            return getNormal().dot(point) + distance;
#endif
        }

        bool contains(const Point<float, 3>& point) const noexcept {
            return std::abs(signedDistance(point)) < std::numeric_limits<float>::epsilon();
        }

        std::string toString() const {
            auto n = getNormal();
            return "Plane(Normal: " + n.toString() + ", Distance: " + std::to_string(distance) + ")";
        }
    };

}
