#pragma once

#include "../Core.h"
#include "SSE/SSE.h"
#include "AVX/AVX.h"

#include <cmath>
#include <string>
#include <sstream>

/**************************
*                         *
*     quaternion<T>       *
*                         *
**************************/

namespace Spindle {

    // all others use scalar
    template <typename T>
    class Quaternion {
    private:
        T x, y, z, w;

    public:
        /**********************
        *    constructors     *
        **********************/
        constexpr Quaternion() noexcept
            : x(T()), y(T()), z(T()), w(T(1)) {}

        constexpr Quaternion(T px, T py, T pz, T pw) noexcept
            : x(px), y(py), z(pz), w(pw) {}

        /**********************
        *      accessors      *
        **********************/
        constexpr T getX() const noexcept { return x; }
        constexpr T getY() const noexcept { return y; }
        constexpr T getZ() const noexcept { return z; }
        constexpr T getW() const noexcept { return w; }

        void setX(T newX) noexcept { x = newX; }
        void setY(T newY) noexcept { y = newY; }
        void setZ(T newZ) noexcept { z = newZ; }
        void setW(T newW) noexcept { w = newW; }

        /**********************
        *  operator overloads *
        **********************/

        // addition
        constexpr Quaternion<T> operator+(const Quaternion<T>& q) const noexcept {
            return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
        }

        // subtraction
        constexpr Quaternion<T> operator-(const Quaternion<T>& q) const noexcept {
            return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
        }

        // multiplication
        constexpr Quaternion<T> operator*(T scalar) const noexcept {
            return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
        }

        // multiplication (Hamilton product)
        constexpr Quaternion<T> operator*(const Quaternion<T>& q) const noexcept {
            return Quaternion(
                w * q.x + x * q.w + y * q.z - z * q.y,
                w * q.y - x * q.z + y * q.w + z * q.x,
                w * q.z + x * q.y - y * q.x + z * q.w,
                w * q.w - x * q.x - y * q.y - z * q.z
            );
        }

        // normalise the quaternion
        constexpr Quaternion<T> normalize() const noexcept {
            T mag = magnitude();
            return (mag > T(0)) ? (*this * (T(1) / mag)) : *this;
        }

        /**********************
        *       methods       *
        **********************/

        // magnitude (length)
        constexpr T magnitude() const noexcept {
            return std::sqrt(x * x + y * y + z * z + w * w);
        }

        // dot product
        constexpr T dot(const Quaternion<T>& q) const noexcept {
            return x * q.x + y * q.y + z * q.z + w * q.w;
        }

        // conjugate
        constexpr Quaternion<T> conjugate() const noexcept {
            return Quaternion(-x, -y, -z, w);
        }

        // inverse
        constexpr Quaternion<T> inverse() const noexcept {
            T magSquared = dot(*this);
            return (magSquared > T(0)) ? conjugate() * (T(1) / magSquared) : *this;
        }

        /**********************
        *       utilities     *
        **********************/

        // string
        std::string toString() const noexcept {
            std::ostringstream oss;
            oss << "(" << x << ", " << y << ", " << z << ", " << w << ")";
            return oss.str();
        }
    };

    // floats use SIMD
    template <>
    class Quaternion<float> {
    private:
        alignas(16) float x, y, z, w;

    public:
        /**********************
        *    constructors     *
        **********************/
        Quaternion() noexcept
            : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

        Quaternion(float px, float py, float pz, float pw) noexcept
            : x(px), y(py), z(pz), w(pw) {}

        /**********************
        *      accessors      *
        **********************/
        float getX() const noexcept { return x; }
        float getY() const noexcept { return y; }
        float getZ() const noexcept { return z; }
        float getW() const noexcept { return w; }

        void setX(float newX) noexcept { x = newX; }
        void setY(float newY) noexcept { y = newY; }
        void setZ(float newZ) noexcept { z = newZ; }
        void setW(float newW) noexcept { w = newW; }

        Quaternion<float> setQuaternion(__m256 result) const noexcept {
            return Quaternion<float>(AVX_GetX(result), AVX_GetY(result), AVX_GetZ(result), AVX_GetW(result));
        }

        Quaternion<float> setQuaternion(__m128 result) const noexcept {
            return Quaternion<float>(SSE_GetX(result), SSE_GetY(result), SSE_GetZ(result), SSE_GetW(result));
        }


        /**********************
        *  operator overloads *
        **********************/

        // SIMD addition
        Quaternion<float> operator+(const Quaternion<float>& q) const noexcept {
#ifdef USE_AVX
            __m256 a = AVX_Set(x, y, z, w, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 b = AVX_Set(q.x, q.y, q.z, q.w, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 result = AVX_Add(a, b);
            return Quaternion<float>(AVX_GetX(result), AVX_GetY(result), AVX_GetZ(result), AVX_GetW(result));
#elif defined(USE_SSE)
            __m128 a = SSE_Set(x, y, z, w);
            __m128 b = SSE_Set(q.x, q.y, q.z, q.w);
            __m128 result = SSE_Add(a, b);
            return Quaternion<float>(SSE_GetX(result), SSE_GetY(result), SSE_GetZ(result), SSE_GetW(result));
#else
            return Quaternion<float>(x + q.x, y + q.y, z + q.z, w + q.w);
#endif
        }

        // SIMD subtraction
        Quaternion<float> operator-(const Quaternion<float>& q) const noexcept {
#ifdef USE_AVX
            __m256 a = AVX_Set(x, y, z, w, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 b = AVX_Set(q.x, q.y, q.z, q.w, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 result = AVX_Subtract(a, b);

            return setQuaternion(result);
#elif defined(USE_SSE)
            __m128 a = SSE_Set(x, y, z, w);
            __m128 b = SSE_Set(q.x, q.y, q.z, q.w);
            __m128 result = SSE_Subtract(a, b);

            return setQuaternion(result);
#else
            return Quaternion<float>(x - q.x, y - q.y, z - q.z, w - q.w);
#endif
        }

        // SIMD multiplication (scalar)
        Quaternion<float> operator*(float scalar) const noexcept {
#ifdef USE_AVX
            __m256 q = AVX_Set(x, y, z, w, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 s = AVX_Set(scalar);
            __m256 result = AVX_Multiply(q, s);
            return setQuaternion(result);
#elif defined(USE_SSE)
            __m128 q = SSE_Set(x, y, z, w);
            __m128 s = SSE_Set1(scalar);
            __m128 result = SSE_Multiply(q, s);
            return setQuaternion(result);
#else
            return Quaternion<float>(x * scalar, y * scalar, z * scalar, w * scalar);
#endif
        }

        // SIMD multiplication (Hamilton product)
        Quaternion<float> operator*(const Quaternion<float>& q) const noexcept {
#ifdef USE_AVX
            __m256 q1 = AVX_Set(x, y, z, w, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 q2 = AVX_Set(q.x, q.y, q.z, q.w, 0.0f, 0.0f, 0.0f, 0.0f);

            // shuffle components for cross terms (y, z, x, w)
            __m256 q1_yzx = AVX_ShuffleYZXW(q1);
            __m256 q2_yzx = AVX_ShuffleYZXW(q2);

            // cross products
            __m256 cross1 = AVX_Multiply(q1, q2_yzx);       // q1 * q2_yzx
            __m256 cross2 = AVX_Multiply(q1_yzx, q2);       // q1_yzx * q2
            __m256  cross = AVX_Subtract(cross1, cross2);   // cross = cross1 - cross2

            // dot product (for w' component)
            __m256    dot = AVX_Multiply(q1, q2);             
            __m256 dotSum = AVX_HorizontalAdd(dot);

            float resultX = AVX_GetW(q1) * AVX_GetX(q2) + AVX_GetX(q1) * AVX_GetW(q2) +
                            AVX_GetY(q1) * AVX_GetZ(q2) - AVX_GetZ(q1) * AVX_GetY(q2);

            float resultY = AVX_GetW(q1) * AVX_GetY(q2) - AVX_GetX(q1) * AVX_GetZ(q2) +
                            AVX_GetY(q1) * AVX_GetW(q2) + AVX_GetZ(q1) * AVX_GetX(q2);

            float resultZ = AVX_GetW(q1) * AVX_GetZ(q2) + AVX_GetX(q1) * AVX_GetY(q2) -
                            AVX_GetY(q1) * AVX_GetX(q2) + AVX_GetZ(q1) * AVX_GetW(q2);

            float resultW = AVX_GetW(q1) * AVX_GetW(q2) - AVX_GetX(q1) * AVX_GetX(q2) -
                            AVX_GetY(q1) * AVX_GetY(q2) - AVX_GetZ(q1) * AVX_GetZ(q2);

            return Quaternion<float>(resultX, resultY, resultZ, resultW);

#elif defined(USE_SSE)
            __m128 q1 = SSE_Set(x, y, z, w);
            __m128 q2 = SSE_Set(q.x, q.y, q.z, q.w);

            __m128 q1_yzx = SSE_ShuffleYZXW(q1);
            __m128 q2_yzx = SSE_ShuffleYZXW(q2);

            __m128 cross1 = SSE_Multiply(q1, q2_yzx);
            __m128 cross2 = SSE_Multiply(q1_yzx, q2);
            __m128  cross = SSE_Subtract(cross1, cross2);

            __m128    dot = SSE_Multiply(q1, q2);
            __m128 dotSum = SSE_HorizontalAdd(dot);

            float resultX = SSE_GetX(cross) + SSE_GetW(dot) - SSE_GetY(cross);
            float resultY = SSE_GetY(cross) - SSE_GetZ(cross) + SSE_GetW(cross) + SSE_GetX(cross);
            float resultZ = SSE_GetZ(cross) + SSE_GetY(dot) - SSE_GetX(cross) + SSE_GetW(cross);
            float resultW = SSE_GetW(dot) - SSE_GetX(dot) - SSE_GetY(dot) - SSE_GetZ(dot);

            return Quaternion<float>(resultX, resultY, resultZ, resultW);


#else
            // Scalar fallback
            return Quaternion<float>(
                w * q.x + x * q.w + y * q.z - z * q.y,
                w * q.y - x * q.z + y * q.w + z * q.x,
                w * q.z + x * q.y - y * q.x + z * q.w,
                w * q.w - x * q.x - y * q.y - z * q.z
            );
#endif
        }

        // normalise
        Quaternion<float> normalize() const noexcept {
            float mag = magnitude();
            return (mag > 0.0f) ? (*this * (1.0f / mag)) : *this;
        }

        /**********************
        *       methods       *
        **********************/

        // magnitude (length)
        float magnitude() const noexcept {
#ifdef USE_AVX
            __m256 q = AVX_Set(x, y, z, w, 0.0f, 0.0f, 0.0f, 0.0f);
            return std::sqrt(AVX_Dot(q, q));
#elif defined(USE_SSE)
            __m128 q = SSE_Set(x, y, z, w);
            return std::sqrt(SSE_Dot(q, q));
#else
            return std::sqrt(x * x + y * y + z * z + w * w);
#endif
        }

        // conjugate
        Quaternion<float> conjugate() const noexcept {
            return Quaternion<float>(-x, -y, -z, w);
        }

        // inverse
        Quaternion<float> inverse() const noexcept {
            float magSquared = dot(*this);
            return (magSquared > 0.0f) ? conjugate() * (1.0f / magSquared) : *this;
        }

        // dot product
        float dot(const Quaternion<float>& q) const noexcept {
#ifdef USE_AVX
            __m256 a = AVX_Set(x, y, z, w, 0.0f, 0.0f, 0.0f, 0.0f);
            __m256 b = AVX_Set(q.x, q.y, q.z, q.w, 0.0f, 0.0f, 0.0f, 0.0f);
            return AVX_Dot(a, b);
#elif defined(USE_SSE)
            __m128 a = SSE_Set(x, y, z, w);
            __m128 b = SSE_Set(q.x, q.y, q.z, q.w);
            return SSE_Dot(a, b);
#else
            return x * q.x + y * q.y + z * q.z + w * q.w;
#endif
        }

        /**********************
        *       utilities     *
        **********************/

        // string
        std::string ToString() const noexcept {
            std::ostringstream oss;
            oss << "(" << x << ", " << y << ", " << z << ", " << w << ")";
            return oss.str();
        }
    };

}