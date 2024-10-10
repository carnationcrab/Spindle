#pragma once

#include <cmath>
#include <cassert>
#include <optional>
#include <xmmintrin.h>  // for SIMD intrinsics
#include <pmmintrin.h>



//_ BASIC MATHS FNS _____________

namespace Spindle {

    //_ MISC __________________________

    // min (overrides system macro)
    #undef MIN
    template <typename T, typename T2>
    constexpr auto MIN(const T a, const T2 b) {
        return a < b ? a : b;
    }

    // max (overrides system macro)
    #undef MAX
    template <typename T, typename T2>
    constexpr auto MAX(const T a, const T2 b) {
        return a > b ? a : b;
    }

    // float abs
    #undef FABS
    template <typename T>
    constexpr T FABS(const T a) {
        return std::fabs(a);
    }
    
    //_ EXPONENTS _____________________
    
    // square root
    template <typename T>
    constexpr T SQRT(const T value) {
        return std::sqrt(value);
    }

    // power
    template <typename T>
    constexpr T POWR(const T base, const T exponent) {
        return std::pow(base, exponent);
    }

    //_ TRIG _________________________
    
    // sine
    template <typename T>
    constexpr T SIN(const T angle) {
        return std::sin(angle);
    }

    // cosine
    template <typename T>
    constexpr T COS(const T angle) {
        return std::cos(angle);
    }

    //_ VECTORS ____________________

    // magnitude
    template <typename T>
    constexpr T MAGN(const T x, const T y) {
        return SQRT(x * x + y * y);
    }

    template <typename T>
    constexpr T MAGN(const T x, const T y, const T z) {
        return SQRT(x * x + y * y + z * z);
    }

    constexpr float EPSILON{ 1e-5 };

    template <typename T>
    constexpr bool sEpsilon(const T value) {
        return FABS(value) < EPSILON;
    }

    // dot product
    inline float SIMD_DotProduct2(__m128 a, __m128 b) noexcept {
        __m128 result = _mm_mul_ps(a, b);                 // Multiply component-wise (x * x, y * y)
        result = _mm_hadd_ps(result, result);             // Horizontal add to sum x and y
        return _mm_cvtss_f32(result);                     // Extract the result as a scalar
    }

    inline float SIMD_DotProduct3(__m128 a, __m128 b) noexcept {
        __m128 result = _mm_mul_ps(a, b);                 // Multiply component-wise (x * x, y * y, z * z)
        result = _mm_hadd_ps(result, result);             // Horizontal add for x and y
        result = _mm_hadd_ps(result, result);             // Add z component to previous result
        return _mm_cvtss_f32(result);                     // Extract the result as a scalar
    }

    // normalization
    inline __m128 SIMD_Normalize2(__m128 vec) noexcept {
        // Dot product of the vector with itself
        __m128 dotProduct = _mm_mul_ps(vec, vec);         // Square components (x * x, y * y)
        dotProduct = _mm_hadd_ps(dotProduct, dotProduct); // Sum x and y

        // Take the reciprocal square root
        __m128 reciprocalSqrt = _mm_rsqrt_ps(dotProduct); // Fast reciprocal square root

        // Normalize by multiplying vector by 1/magnitude
        return _mm_mul_ps(vec, _mm_shuffle_ps(reciprocalSqrt, reciprocalSqrt, _MM_SHUFFLE(0, 0, 0, 0)));
    }

    inline __m128 SSE_Normalize(__m128 vec) noexcept {
                                                          // dot product of the vector with itself
        __m128 dotProduct = _mm_mul_ps(vec, vec);         //    square components (x * x, y * y, z * z)
        dotProduct = _mm_hadd_ps(dotProduct, dotProduct); //    sum x and y
        dotProduct = _mm_hadd_ps(dotProduct, dotProduct); //    add z to the previous result

                                                          // take the reciprocal square root
        __m128 reciprocalSqrt = _mm_rsqrt_ps(dotProduct); //    fast reciprocal square root TODO ADD NEWTON RATHWHATEVER THING

                                                          // normalize by multiplying vector by 1/magnitude
        return _mm_mul_ps(vec, 
            _mm_shuffle_ps(reciprocalSqrt, reciprocalSqrt, _MM_SHUFFLE(0, 0, 0, 0)));
    }

    // vector addition
    inline __m128 SSE_Add(__m128 a, __m128 b) noexcept {
        return _mm_add_ps(a, b);
    }

    // vector subtraction
    inline __m128 SSE_Subtract(__m128 a, __m128 b) noexcept {
        return _mm_sub_ps(a, b);  
    }

    // bector multiplication
    inline __m128 SSE_Multiply(__m128 vec, float scalar) noexcept {
        __m128 scalarVec = _mm_set1_ps(scalar);           // Broadcast scalar across all SIMD lanes
        return _mm_mul_ps(vec, scalarVec);                // Perform SIMD multiplication
    }

    inline __m128 SSE_Cross(__m128 a, __m128 b) noexcept {
        __m128 a_yzx = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));      // shuffle components to prepare
        __m128 b_yzx = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1));      // for cross product calculation

        __m128 c = _mm_sub_ps(_mm_mul_ps(a, b_yzx), _mm_mul_ps(a_yzx, b)); // perform cross product

        return _mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 0, 2, 1));;
    }
}

