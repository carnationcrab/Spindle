#pragma once
#define _USE_MATH_DEFINES

#include "Point.h"
#include <cmath>
#include <string>
#include "SSE/SSE.h"
#include "AVX/AVX.h"

namespace Spindle {

    template <typename T>
    class Sphere {
    private:
        Point<T, 3> centre;
        T radius;

    public:
        constexpr Sphere() noexcept : centre(), radius(0) {}
        Sphere(const Point<T, 3>& c, T r) : centre(c), radius(r) {}

        const Point<T, 3>& getCentre() const noexcept { return centre; }
        void setCentre(const Point<T, 3>& c) noexcept { centre = c; }

        T getRadius() const noexcept { return radius; }
        void setRadius(T r) noexcept { radius = r; }

        bool contains(const Point<T, 3>& point) const noexcept {
            return (point - centre).magnitudeSquared() <= radius * radius;
        }

        bool intersects(const Sphere& other) const noexcept {
            T distanceSquared = (centre - other.centre).magnitudeSquared();
            T radiusSum = radius + other.radius;
            return distanceSquared <= radiusSum * radiusSum;
        }

        T volume() const noexcept {
            return (4.0 / 3.0) * M_PI * radius * radius * radius;
        }

        T surfaceArea() const noexcept {
            return 4.0 * M_PI * radius * radius;
        }

        std::string toString() const {
            return "Sphere(Centre: " + centre.toString() + ", Radius: " + std::to_string(radius) + ")";
        }
    };

    template <>
    class Sphere<float> {
    private:
#ifdef USE_AVX
        __m256 centre;  // packed [cx, cy, cz, 0.0]
        float radius;
#elif defined(USE_SSE)
        __m128 centre;  // packed [cx, cy, cz, 0.0]
        float radius;
#else
        Point<float, 3> centre;
        float radius;
#endif

    public:
        Sphere() noexcept {
#ifdef USE_AVX
            centre = AVX_Set(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            radius = 0.0f;
#elif defined(USE_SSE)
            centre = SSE_Set(0.0f, 0.0f, 0.0f, 0.0f);
            radius = 0.0f;
#else
            centre = Point<float, 3>();
            radius = 0.0f;
#endif
        }

        Sphere(const Point<float, 3>& c, float r) {
#ifdef USE_AVX
            centre = AVX_Set(c.x, c.y, c.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            radius = r;
#elif defined(USE_SSE)
            centre = SSE_Set(c.x, c.y, c.z, 0.0f);
            radius = r;
#else
            centre = c;
            radius = r;
#endif
        }

        Point<float, 3> getCentre() const noexcept {
#ifdef USE_AVX
            return Point<float, 3>(AVX_GetX(centre), AVX_GetY(centre), AVX_GetZ(centre));
#elif defined(USE_SSE)
            return Point<float, 3>(SSE_GetX(centre), SSE_GetY(centre), SSE_GetZ(centre));
#else
            return centre;
#endif
        }

        void setCentre(const Point<float, 3>& c) noexcept {
#ifdef USE_AVX
            centre = AVX_Set(c.x, c.y, c.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
#elif defined(USE_SSE)
            centre = SSE_Set(c.x, c.y, c.z, 0.0f);
#else
            centre = c;
#endif
        }

        float getRadius() const noexcept { return radius; }
        void setRadius(float r) noexcept { radius = r; }

        bool contains(const Point<float, 3>& point) const noexcept {
#ifdef USE_AVX
            __m256 p = AVX_Set(point.x, point.y, point.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 diff = AVX_Subtract(p, centre);
            float distSquared = AVX_Dot(diff, diff);
            return distSquared <= radius * radius;
#elif defined(USE_SSE)
            __m128 p = SSE_Set(point.x, point.y, point.z, 0.0f);
            __m128 diff = SSE_Subtract(p, centre);
            float distSquared = SSE_Dot(diff, diff);
            return distSquared <= radius * radius;
#else
            return (point - getCentre()).magnitudeSquared() <= radius * radius;
#endif
        }

        bool intersects(const Sphere<float>& other) const noexcept {
#ifdef USE_AVX
            __m256 diff = AVX_Subtract(centre, other.centre);
            float distSquared = AVX_Dot(diff, diff);
            float radiusSum = radius + other.radius;
            return distSquared <= radiusSum * radiusSum;
#elif defined(USE_SSE)
            __m128 diff = SSE_Subtract(centre, other.centre);
            float distSquared = SSE_Dot(diff, diff);
            float radiusSum = radius + other.radius;
            return distSquared <= radiusSum * radiusSum;
#else
            return (getCentre() - other.getCentre()).magnitudeSquared() <= (radius + other.getRadius()) * (radius + other.getRadius());
#endif
        }

        float volume() const noexcept {
            return (4.0f / 3.0f) * M_PI * radius * radius * radius;
        }

        float surfaceArea() const noexcept {
            return 4.0f * M_PI * radius * radius;
        }

        std::string toString() const {
            auto c = getCentre();
            return "Sphere(Centre: " + c.toString() + ", Radius: " + std::to_string(radius) + ")";
        }
    };

}
