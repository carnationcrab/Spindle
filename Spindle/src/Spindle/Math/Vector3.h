#pragma once

#include "../Core.h"
#include "baseMaths.h"

#include <sstream>  // For std::stringstream
#include <string>   // For std::string
#include <xmmintrin.h> 

// overcommented for reference purposes, sorry

#include <iostream>

namespace Spindle {
    template <typename T>
    struct alignas(16) Vector3 {
    private:
        // epsilon (for floating-point comparison)

        constexpr Vector3 formatSSEVector(__m128 sseInt) const {
            return Vector3(_mm_cvtss_f32(sseInt),
                _mm_cvtss_f32(_mm_shuffle_ps(sseInt, sseInt, _MM_SHUFFLE(1, 1, 1, 1))),
                _mm_cvtss_f32(_mm_shuffle_ps(sseInt, sseInt, _MM_SHUFFLE(2, 2, 2, 2))));
        }

    public:
        T x, y, z;

        constexpr Vector3() noexcept 
            :x(T()), y(T()), z(T()) {}    
        constexpr Vector3(T p_x, T p_y, T p_z) noexcept 
            :x(p_x), y(p_y), z(p_z) {}

        //// move
        //Vector3(Vector3&& other) noexcept : x(other.x), y(other.y), z(other.z) {
        //    other.x = other.y = other.z = T();
        //}

        // magnitude
        constexpr T magnitude() const noexcept {
            return SQRT(magnitudeSquared());
        }

        // magnitude squared
        constexpr T magnitudeSquared() const noexcept {
            return x * x + y * y + z * z;
        }

        // normalize (sse)
        constexpr Vector3 normalize() const noexcept {
            __m128 vec = _mm_setr_ps(x, y, z, 0.0f);
            __m128 mag = SIMD_Magnitude3(vec);
            __m128 recip_mag = _mm_rcp_ps(mag);
            // refine with Newton-Raphson
            recip_mag = _mm_mul_ps(
                _mm_sub_ps(
                    _mm_set1_ps(2.0f), 
                    _mm_mul_ps(mag, recip_mag)
                ), 
                   recip_mag);

            __m128 result = _mm_mul_ps(vec, recip_mag);
            return formatSSEVector(result);
        }


        // addition (SSE)
        constexpr Vector3 operator+(const Vector3& operand) const noexcept {
            __m128 a = _mm_setr_ps(x, y, z, 0.0f);
            __m128 b = _mm_setr_ps(operand.x, operand.y, operand.z, 0.0f);
            __m128 result = SSE_Add(a, b);

            return formatSSEVector(result);
        }
                                                                                      
        // subtraction (SSE)                                                         
        constexpr Vector3 operator-(const Vector3& operand) const noexcept {
            __m128 a = _mm_setr_ps(x, y, z, 0.0f);
            __m128 b = _mm_setr_ps(operand.x, operand.y, operand.z, 0.0f);
            __m128 result = SSE_Subtract(a, b);

            return formatSSEVector(result);
        }
                                                                                      
        // multiplication (SSE)                                                      
        constexpr Vector3 operator*(float scalar) const noexcept {
            __m128 vec = _mm_setr_ps(x, y, z, 0.0f);
            __m128 result = SSE_Multiply(vec, scalar);

            return formatSSEVector(result);
        }

        // comparison (SSE)
        bool operator<(const Vector3& operand) const {
            __m128 v1 = _mm_set_ps(0.0f, z, y, x);
            __m128 v2 = _mm_set_ps(0.0f, operand.z, operand.y, operand.x);
            __m128 cmp = _mm_cmplt_ps(v1, v2);
            return _mm_movemask_ps(cmp) == 0x07; // check the 
        }


        bool operator>(const Vector3<T>& operand) const {
            __m128 v1 = _mm_set_ps(0.0f, z, y, x);
            __m128 v2 = _mm_set_ps(0.0f, operand.z, operand.y, operand.x);
            __m128 cmp = _mm_cmpgt_ps(v1, v2);
            return _mm_movemask_ps(cmp) == 0x07; // Check if all three components are greater
        }
                                                                                      
        // dot product (SSE)
        // could maybe be optimized with FMA (Fused Multiply-Add) later                                                            
        constexpr float dot(const Vector3& operand) const noexcept {
            __m128 a = _mm_setr_ps(x, y, z, 0.0f);        
            __m128 b = _mm_setr_ps(operand.x, operand.y, operand.z, 0.0f); 
            return SIMD_DotProduct3(a, b);               
        }


        // cross product (SSE) (three shuffles) see: https://threadlocalmutex.com/?p=8
        Vector3 cross(__m128 a, __m128 b) const noexcept {
            __m128 a = _mm_setr_ps(x, y, z, 0.0f);
            __m128 b = _mm_setr_ps(operand.x, operand.y, operand.z, 0.0f);

            result{ SSE_Cross(a, b) };

            T components[4];
            _mm_store_ps(components, result); // store results in array

            return Vector3(components[0], components[1], components[2]); // return only the x, y, z components (ignoring w)
        }

        // checks
        constexpr bool isCollinear(const Vector3& operand) const noexcept {
            return FABS(dot(operand) - (magnitude() * operand.magnitude())) < EPSILON;
        }

        constexpr bool isPerpendicular(const Vector3& operand) const noexcept {
            return FABS(dot(operand)) < EPSILON;
        }

        // (a * b) > 0
        constexpr bool isSameDirection(const Vector3& operand) const noexcept {
            return dot(operand) > 0;
        }

        // print
        void print() const noexcept {
            std::cout << "Vector3(" << x << ", " << y << ", " << z << ") | Magnitude: " << magnitude() << std::endl;
        }

        std::string toString() const noexcept {
            std::stringstream ss;
            ss << "Vector3(" << x << ", " << y << ", " << z << ")";
            return ss.str();
        }

    };
}
