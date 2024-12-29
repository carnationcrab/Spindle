#pragma once

#include "../Core.h"
#include "SSE/SSE.h"
#include "AVX/AVX.h"

#include <cmath>
#include <string>

/**************************
*                         *
*    Vector (Template)    *
*                         *
**************************/

namespace Spindle {

    template <typename T, size_t Dimension>
    struct Vector;

    /********************************
    *                               *
    *     2D Vector Specialization  *
    *                               *
    ********************************/
    template <typename T>
    struct Vector<T, 2> {
        T x, y;

        /**********************
        *    Constructors     *
        **********************/

        constexpr Vector() noexcept : x{ T() }, y{ T() } {}
        constexpr Vector(T px, T py) noexcept : x{ px }, y{ py } {}

        /**********************
        *  Operator Overloads *
        **********************/

        constexpr Vector operator+(const Vector& operand) const noexcept {
            return Vector(x + operand.x, y + operand.y);
        }

        constexpr Vector operator-(const Vector& operand) const noexcept {
            return Vector(x - operand.x, y - operand.y);
        }

        constexpr Vector operator*(T scalar) const noexcept {
            return Vector(x * scalar, y * scalar);
        }

        /**********************
        *       Methods       *
        **********************/

        T dot(const Vector& operand) const noexcept {
            return x * operand.x + y * operand.y;
        }

        T magnitudeSquared() const noexcept {
            return dot(*this);
        }

        T magnitude() const noexcept {
            return std::sqrt(magnitudeSquared());
        }

        Vector unitVector() const noexcept {
            T mag = magnitude();
            return Vector(x / mag, y / mag);
        }

        bool isCollinear(const Vector& operand, T epsilon = 1e-5) const noexcept {
            T dotProduct = dot(operand);
            return std::abs(dotProduct - magnitude() * operand.magnitude()) < epsilon;
        }

        bool isPerpendicular(const Vector& operand, T epsilon = 1e-5) const noexcept {
            return std::abs(dot(operand)) < epsilon;
        }

        std::string toString() const {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }

        T getX() const noexcept { return x; }
        void setX(T newX) noexcept { x = newX; }

        T getY() const noexcept { return y; }
        void setY(T newY) noexcept { y = newY; }
    };

    /********************************
    *                               *
    *     3D Vector Specialization  *
    *                               *
    ********************************/
    template <typename T>
    struct Vector<T, 3> {
        T x, y, z;

        /**********************
        *    Constructors     *
        **********************/

        constexpr Vector() noexcept : x{ T() }, y{ T() }, z{ T() } {}
        constexpr Vector(T px, T py, T pz) noexcept : x{ px }, y{ py }, z{ pz } {}

        /**********************
        *  Operator Overloads *
        **********************/

        constexpr Vector operator+(const Vector& operand) const noexcept {
            return Vector(x + operand.x, y + operand.y, z + operand.z);
        }

        constexpr Vector operator-(const Vector& operand) const noexcept {
            return Vector(x - operand.x, y - operand.y, z - operand.z);
        }

        constexpr Vector operator*(T scalar) const noexcept {
            return Vector(x * scalar, y * scalar, z * scalar);
        }

        /**********************
        *       Methods       *
        **********************/

        T dot(const Vector& operand) const noexcept {
            return x * operand.x + y * operand.y + z * operand.z;
        }

        Vector cross(const Vector& operand) const noexcept {
            return Vector(
                y * operand.z - z * operand.y,
                z * operand.x - x * operand.z,
                x * operand.y - y * operand.x
            );
        }

        T magnitudeSquared() const noexcept {
            return dot(*this);
        }

        T magnitude() const noexcept {
            return std::sqrt(magnitudeSquared());
        }

        Vector unitVector() const noexcept {
            T mag = magnitude();
            return Vector(x / mag, y / mag, z / mag);
        }

        bool isCollinear(const Vector& operand, T epsilon = 1e-5) const noexcept {
            T dotProduct = dot(operand);
            return std::abs(dotProduct - magnitude() * operand.magnitude()) < epsilon;
        }

        bool isPerpendicular(const Vector& operand, T epsilon = 1e-5) const noexcept {
            return std::abs(dot(operand)) < epsilon;
        }

        std::string toString() const {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
        }

        T getX() const noexcept { return x; }
        void setX(T newX) noexcept { x = newX; }

        T getY() const noexcept { return y; }
        void setY(T newY) noexcept { y = newY; }

        T getZ() const noexcept { return z; }
        void setZ(T newZ) noexcept { z = newZ; }
    };

}
