#pragma once

#include "Line.h"
#include "Point.h"
#include "Vector.h"

namespace Spindle {

    /********************************
    *                               *
    *     ray (a to infinity)       *
    *                               *
    ********************************/

    template <typename T, size_t Dimension>
    struct Ray {
        Line<T, Dimension> line;

        /**********************
        *    constructors     *
        **********************/

        constexpr Ray() noexcept
            : line() {}

        Ray(const Point<T, Dimension>& origin, const Vector<T, Dimension>& dir)
            : line(origin, dir.unitVector()) {}

        /**********************
        *       methods       *
        **********************/

        // computes a point on the ray for a given scalar parameter t (t >= 0)
        Point<T, Dimension> getPoint(T t) const noexcept {
            assert(t >= 0.0 && t <= 1.0 && "parameter t must be in the range [0, 1] for a line segment.");

            // TODO in production, clamp the value
            // t = std::max(T(0.0), std::min(t, T(1.0)));

            return line.getPoint(t);
        }

        // checks if two rays are equal
        bool operator==(const Ray& other) const noexcept {
            return line == other.line;
        }

        bool operator!=(const Ray& other) const noexcept {
            return !(*this == other);
        }

        /**********************
        *      utilities      *
        **********************/

        std::string toString() const {
            return "Ray(" + line.toString() + ")";
        }
    };

    /********************************
    *                               *
    *   ray<float, Dimension>       *
    *                               *
    ********************************/

    template <size_t Dimension>
    struct Ray<float, Dimension> {
        Line<float, Dimension> line;

        /**********************
        *    constructors     *
        **********************/

        constexpr Ray() noexcept
            : line() {}

        Ray(const Point<float, Dimension>& origin, const Vector<float, Dimension>& dir)
            : line(origin, dir.unitVector()) {}

        /**********************
        *       methods       *
        **********************/

        // computes a point on the ray for a given scalar parameter t (t >= 0)
        Point<float, Dimension> getPoint(float t) const noexcept {
            assert(t >= 0.0f && "parameter t must be in the range [0, 1] for a line segment.");
            
            return line.getPoint(t);
        }

        bool operator==(const Ray& other) const noexcept {
            return line == other.line;
        }

        bool operator!=(const Ray& other) const noexcept {
            return !(*this == other);
        }

        /**********************
        *      utilities      *
        **********************/

        std::string toString() const {
            return "Ray(" + line.toString() + ")";
        }
    };
}
