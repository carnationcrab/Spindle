#pragma once

#include "../Core.h"
#include "SSE.h"

#include <cmath>
#include <string>

/********************************
*                               *
*  Vector (3D)      (2,1,5)     *
*                               *
********************************/

namespace Spindle {

    // all other datatypes use regular math
    template <typename T>
    struct Vector3 {
    private:
        T x, y, z;


    /**********************
    *                     *
    *    constructors     *
    *                     *
    **********************/

    public:
        constexpr Vector3() noexcept
            : x{ T() },
              y{ T() },
              z{ T() } {}

        constexpr Vector3(T p_x, T p_y, T p_z) noexcept
            : x{ p_x },
              y{ p_y },
              z{ p_z } {}

    /**********************
    *                     *
    * operator overloads  *
    *                     *
    **********************/

        // addition
        constexpr Vector3 operator+(const Vector3& operand) const noexcept {
            return Vector3(x + operand.x, y + operand.y, z + operand.z);
        }

        // subtraction
        constexpr Vector3 operator-(const Vector3& operand) const noexcept {
            return Vector3(x - operand.x, y - operand.y, z - operand.z);
        }

        // multiplication by a scalar
        constexpr Vector3 operator*(T scalar) const noexcept {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }

    /**********************
    *                     *
    *       methods       *
    *                     *
    **********************/

        // dot product
        constexpr T dot(const Vector3& operand) const noexcept {
            return (x * operand.x) + (y * operand.y) + (z * operand.z);
        }

        // cross product
        constexpr Vector3 cross(const Vector3& operand) const noexcept {
            return Vector3(
                (y * operand.z) - (z * operand.y),
                (z * operand.x) - (x * operand.z),
                (x * operand.y) - (y * operand.x)
            );
        }

        // magnitude squared
        constexpr T magnitudeSquared() const noexcept {
            return dot(*this);
        }

        // magnitude
        T magnitude() const noexcept {
            return std::sqrt(magnitudeSquared());
        }

        // collinear test (same direction)
        bool isCollinear(const Vector3& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);
            float       magA = magnitude();
            float       magB = operand.magnitude();

            return std::abs(dotProduct - (magA * magB)) < epsilon;
        }

        // collinear but opposite
        bool isCollinearOpposite(const Vector3& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);
            float       magA = magnitude();
            float       magB = operand.magnitude();

            return std::abs(dotProduct + (magA * magB)) < epsilon;
        }

        // perpendicular test
        bool isPerpendicular(const Vector3& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);

            return std::abs(dotProduct) < epsilon;
        }

        // same direction
        bool isSameDirection(const Vector3& operand) const noexcept {
            return dot(operand) > 0;
        }

        // opposite direction
        constexpr bool isOppositeDirection(const Vector3& operand) const noexcept {
            return dot(operand) < 0;
        }


    /**********************
    *                     *
    *      utilities      *
    *                     *
    **********************/

        // string output
        std::string toString() const {
            return "(" + std::to_string(x) + ", " 
                       + std::to_string(y) + ", " 
                       + std::to_string(z) + ")";
        }

        // getters for x, y, and z
        constexpr    T getX() const noexcept { return x; }
        constexpr    T getY() const noexcept { return y; }
        constexpr    T getZ() const noexcept { return z; }

        // setters for x, y, and z
        constexpr void setX(T newX) noexcept { x = newX; }
        constexpr void setY(T newY) noexcept { y = newY; }
        constexpr void setZ(T newZ) noexcept { z = newZ; }
    };

/*************************************
*                                    *
*  Vector3<float> (3D)(2.0,1.0,5.0)  *
*                                    *
*************************************/

    // Vector3<float> uses SIMD
    template <>
    struct alignas(16) Vector3<float> {
    private:
        float x, y, z;


    /**********************
    *                     *
    *    constructors     *
    *                     *
    **********************/

    public:
        Vector3() noexcept
            : x{ 0.0f }, 
              y{ 0.0f }, 
              z{ 0.0f } {}

        Vector3(float p_x, float p_y, float p_z) noexcept
            : x{ p_x },
              y{ p_y },
              z{ p_z } {}

    /**********************
    *                     *
    * operator overloads  *
    *                     *
    **********************/

        // addition (SIMD)
        Vector3 operator+(const Vector3<float>& operand) const noexcept {
            __m128      a = SSE_Set(x, y, z, 0.0f);
            __m128      b = SSE_Set(operand.x, operand.y, operand.z, 0.0f);
            __m128 result = SSE_Add(a, b);
            
            return Vector3(SSE_GetX(result), SSE_GetY(result), _mm_cvtss_f32(_mm_shuffle_ps(result, result, _MM_SHUFFLE(2, 2, 2, 2))));
        }

        // subtraction (SIMD)
        Vector3 operator-(const Vector3<float>& operand) const noexcept {
            __m128      a = SSE_Set(x, y, z, 0.0f);
            __m128      b = SSE_Set(operand.x, operand.y, operand.z, 0.0f);
            __m128 result = SSE_Subtract(a, b);
            
            return Vector3(
                SSE_GetX(result),
                SSE_GetY(result),
                SSE_GetZ(result));
        }

        // multiplication by a scalar (SIMD)
        Vector3 operator*(float scalar) const noexcept {
            __m128 a = SSE_Set(x, y, z, 0.0f);
            __m128 result = SSE_Multiply(a, scalar);
            return Vector3(
                SSE_GetX(result),
                SSE_GetY(result),
                SSE_GetZ(result));
        }

    /**********************
    *                     *
    *       methods       *
    *                     *
    **********************/

        // normalized vector, unit vector
        Vector3 unitVector() const noexcept {
            __m128      a = SSE_Set(x, y, z, 0.0f);
            __m128 result = SSE_Multiply(a, (1 / magnitude()));

            return setVector3(result);
        }

        // cross product (not SIMD accelerated)
        Vector3 cross(const Vector3<float>& operand) const noexcept {
            return Vector3(
                (y * operand.z) - (z * operand.y),
                (z * operand.x) - (x * operand.z),
                (x * operand.y) - (y * operand.x) 
            );
        }

        // dot product (SIMD)
        float dot(const Vector3<float>& operand) const noexcept {
            __m128 a = SSE_Set(x, y, z, 0.0f);
            __m128 b = SSE_Set(operand.x, operand.y, operand.z, 0.0f);
            
            return SSE_Dot(a, b);
        }

        // magnitude squared constexpr?
        float magnitudeSquared() const noexcept { return dot(*this); }

        // magnitude
        float magnitude() const noexcept {
            return std::sqrt(magnitudeSquared());
        }

        // Collinear test (same direction)
        bool isCollinear(const Vector3& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);
            float       magA = magnitude();
            float       magB = operand.magnitude();
            
            return std::abs(dotProduct - (magA * magB)) < epsilon;
        }

        // Collinear but opposite
        bool isCollinearOpposite(const Vector3& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);
            float       magA = magnitude();
            float       magB = operand.magnitude();
            
            return std::abs(dotProduct + (magA * magB)) < epsilon;
        }

        // Perpendicular test
        bool isPerpendicular(const Vector3& operand, float epsilon = 1e-5) const noexcept {
            float dotProduct = dot(operand);
            return std::abs(dotProduct) < epsilon;
        }

        // Same direction
        bool isSameDirection(const Vector3& operand) const noexcept {
            return dot(operand) > 0;
        }

        // Opposite direction
        bool isOppositeDirection(const Vector3& operand) const noexcept {
            return dot(operand) < 0;
        }

    /**********************
    *                     *
    *      utilities      *
    *                     *
    **********************/

        // "(x, y, z)"
        std::string toString() const noexcept {
            return "(" + 
                std::to_string(x) + ", "
              + std::to_string(y) + ", " 
              + std::to_string(z) + ")";
        }


        Vector3 setVector3(__m128 result) const noexcept {
            return Vector3(
                SSE_GetX(result),
                SSE_GetY(result),
                SSE_GetZ(result)
            );
        }

        // getters
        float getX()     const noexcept { return x; }
        void  setX(float newX) noexcept { x = newX; }

        float getY()     const noexcept { return y; }
        void  setY(float newY) noexcept { y = newY; }
        
        float getZ()     const noexcept { return z; }
        void  setZ(float newZ) noexcept { z = newZ; }
    };
}
