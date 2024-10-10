#pragma once

#include <xmmintrin.h>

namespace Spindle {

    // initializes an __m128 variable with the four
    // floating point values provided
    inline __m128 SSE_Set(float x, float y, float z, float w) noexcept {
        return _mm_set_ps(w, z, y, x);
    }

    // load four floats from c-style array into an __m128
    // variable (16-byte aligned)
    inline __m128 SSE_Load(const float* data) noexcept {
        return _mm_load_ps(data);
    }

    // stores contents of __m128 variable in 
    // c-style array of four floats (16-byte aligned)
    inline void SSE_Store(float* data, __m128 v) noexcept {
        _mm_store_ps(data, v);
    }

    // extracts the first element (index 0) from 
    // an __m128 vector
    inline float SSE_GetX(__m128 v) noexcept {
        return _mm_cvtss_f32(v);
    }

    // extracts the second element (index 1) from 
    // an __m128 vector
    inline float SSE_GetY(__m128 v) noexcept {
        return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
    }

    // vector addition in parallel
    inline __m128 SSE_Add(__m128 a, __m128 b) noexcept {
        return _mm_add_ps(a, b);
    }

    // vector subtraction in parallel
    inline __m128 SSE_Subtract(__m128 a, __m128 b) noexcept {
        return _mm_sub_ps(a, b);
    }

    // vector multiplication in parallel
    inline __m128 SSE_MultiplyScalar(__m128 vec, float scalar) noexcept {
        __m128 scalarVec = _mm_set1_ps(scalar);           // Broadcast scalar across all SIMD lanes
        return _mm_mul_ps(vec, scalarVec);                // Perform SIMD multiplication
    }

    inline __m128 SSE_Multiply(__m128 a, __m128 b) noexcept {
        return _mm_mul_ps(a, b);
    }

    // computes the dot product using transposed loading approach
    // for large data
    inline __m128 SSE_Dot_TL(const float* a, const float* b) noexcept {
        // Load the vectors in transposed order
        __m128 vaX = SSE_Load(&a[0]);  // a[0, 4, 8, 12]
        __m128 vaY = SSE_Load(&a[1]);  // a[1, 5, 9, 13]
        __m128 vaZ = SSE_Load(&a[2]);  // a[2, 6, 10, 14]
        __m128 vaW = SSE_Load(&a[3]);  // a[3, 7, 11, 15]

        __m128 vbX = SSE_Load(&b[0]);  // b[0, 4, 8, 12]
        __m128 vbY = SSE_Load(&b[1]);  // b[1, 5, 9, 13]
        __m128 vbZ = SSE_Load(&b[2]);  // b[2, 6, 10, 14]
        __m128 vbW = SSE_Load(&b[3]);  // b[3, 7, 11, 15]

        // perform element-wise multiplication and accumulate the results
        __m128 result = SSE_Multiply(vaX, vbX);
        result = SSE_Add(result, SSE_Multiply(vaY, vbY));
        result = SSE_Add(result, SSE_Multiply(vaZ, vbZ));
        result = SSE_Add(result, SSE_Multiply(vaW, vbW));

        return result;
    }

    // dot product of two vectors
    inline float SSE_Dot(__m128 a, __m128 b) noexcept {
        // perform element-wise multiplication
        __m128 multResult = SSE_Multiply(a, b);

        // horizontally add the results
        return SSE_GetX(multResult) + SSE_GetY(multResult);
    }
}