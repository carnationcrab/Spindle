#pragma once

#include "../Core.h"
#include "baseMaths.h"

#include <iostream>
#include <cmath> // Include cmath for std::sqrt and std::fabs

namespace Spindle {
    template <typename T>
    struct Vector2 {
        T x, y;

        Vector2(T p_x = T(), T p_y = T())
            : x(p_x), y(p_y) {}

        // magnitude
        T magnitude() const {
            return MAGN(x, y); // Correct magnitude calculation
        }

        // magnitude squared
        T magnitudeSquared() const {
            return x * x + y * y;
        }

        // addition
        Vector2 operator+(const Vector2& operand) const {
            return Vector2(x + operand.x, y + operand.y);
        }

        // subtraction
        Vector2 operator-(const Vector2& operand) const {
            return Vector2(x - operand.x, y - operand.y);
        }

        // scalar multiplication
        Vector2 operator*(float scalar) const {
            return Vector2(x * scalar, y * scalar);
        }

        // dot product
        float dot(const Vector2& operand) const {
            return x * operand.x + y * operand.y; // Ensure correct dot product calculation
        }

        // Epsilon for floating-point comparison
        const T EPSILON = 1e-6; // Adjust as necessary

        // (a * b) = ab
        bool isCollinear(const Vector2& operand) const {
            T dotProduct = this->dot(operand);
            return FABS(dotProduct * dotProduct - (this->magnitudeSquared() * operand.magnitudeSquared())) < EPSILON;
        }

        // (a * b) = -ab
        bool isCollinearButOpposite(const Vector2& operand) const {
            T dotProduct = this->dot(operand);
            return FABS(dotProduct * dotProduct + (this->magnitudeSquared() * operand.magnitudeSquared())) < EPSILON;
        }

        // (a * b) = 0
        bool isPerpendicular(const Vector2& operand) const {
            T dotProduct = this->dot(operand);
            return FABS(dotProduct) < EPSILON;
        }

        // (a * b) > 0
        bool isSameDirection(const Vector2& operand) const {
            T dotProduct = this->dot(operand);
            return dotProduct > T();
        }

        // (a * b) < 0
        bool isOppositeDirection(const Vector2& operand) const {
            return !isSameDirection(operand);
        }

        void print() const {
            std::cout << "Vector2(" << x << ", " << y << ") | Magnitude: " << magnitude() << std::endl;
        }
    };


    template <typename T>
    struct Vector3 {
        T x, y, z;

        Vector3(T p_x = T(), T p_y = T(), T p_z = T())
            : x(p_x), y(p_y), z(p_z) {}

        // magnitude
        T magnitude() const {
            return MAGN(x,y,z); // Correct magnitude calculation
        }

        // magnitude squared
        T magnitudeSquared() const {
            return x * x + y * y + z * z; // Correct calculation
        }

        // addition
        Vector3 operator+(const Vector3& operand) const {
            return Vector3(x + operand.x, y + operand.y, z + operand.z);
        }

        // subtraction
        Vector3 operator-(const Vector3& operand) const {
            return Vector3(x - operand.x, y - operand.y, z - operand.z);
        }

        float dot(const Vector3& operand) const {
            return x * operand.x + y * operand.y + z * operand.z; // Ensure correct dot product calculation
        }

        // Epsilon for floating-point comparison
        const T EPSILON = 1e-6; // Adjust as necessary

        // (a * b) = ab
        bool isCollinear(const Vector3& operand) const {
            T dotProduct = this->dot(operand);
            return FABS(dotProduct * dotProduct - (this->magnitudeSquared() * operand.magnitudeSquared())) < EPSILON;
        }

        // (a * b) = -ab
        bool isCollinearButOpposite(const Vector3& operand) const {
            T dotProduct = this->dot(operand);
            return FABS(dotProduct * dotProduct + (this->magnitudeSquared() * operand.magnitudeSquared())) < EPSILON;
        }

        // (a * b) = 0
        bool isPerpendicular(const Vector3& operand) const {
            T dotProduct = this->dot(operand);
            return FABS(dotProduct) < EPSILON;
        }

        // (a * b) > 0
        bool isSameDirection(const Vector3& operand) const {
            T dotProduct = this->dot(operand);
            return dotProduct > T();
        }

        // (a * b) < 0
        bool isOppositeDirection(const Vector3& operand) const {
            return !isSameDirection(operand);
        }

        void print() const {
            std::cout << "Vector3(" << x << ", " << y << ", " << z << ") | Magnitude: " << magnitude() << std::endl;
        }
    };
}
