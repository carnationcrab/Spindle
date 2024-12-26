#pragma once

#include <immintrin.h> // AVX intrinsics

// AVX utilities for 256-bit SIMD operations
namespace Spindle {

    /******************************
    *                             *
    *     AVX Initialization      *
    *                             *
    ******************************/

    // Initializes an __m256 variable with eight float values
    inline __m256 AVX_Set(float x, float y, float z, float w, float a, float b, float c, float d) noexcept {
        return _mm256_set_ps(d, c, b, a, w, z, y, x);
    }

    // Broadcast a single float across all lanes of an __m256 variable
    inline __m256 AVX_Set(float value) noexcept {
        return _mm256_set1_ps(value);
    }

    // Loads eight floats from a 32-byte aligned array into an __m256 variable
    inline __m256 AVX_Load(const float* data) noexcept {
        return _mm256_load_ps(data);
    }

    // Stores the contents of an __m256 variable into a 32-byte aligned array
    inline void AVX_Store(float* data, __m256 v) noexcept {
        _mm256_store_ps(data, v);
    }

    /******************************
    *                             *
    *     AVX Arithmetic Ops      *
    *                             *
    ******************************/

    // Vector addition
    inline __m256 AVX_Add(__m256 a, __m256 b) noexcept {
        return _mm256_add_ps(a, b);
    }

    // Vector subtraction
    inline __m256 AVX_Subtract(__m256 a, __m256 b) noexcept {
        return _mm256_sub_ps(a, b);
    }

    // Vector multiplication
    inline __m256 AVX_Multiply(__m256 a, __m256 b) noexcept {
        return _mm256_mul_ps(a, b);
    }

    // Scalar multiplication (broadcast scalar to all lanes, then multiply)
    inline __m256 AVX_Multiply(__m256 vec, float scalar) noexcept {
        __m256 scalarVec = AVX_Set(scalar);
        return _mm256_mul_ps(vec, scalarVec);
    }

    // Fused multiply-add: (a * b) + c
    inline __m256 AVX_MultiplyAdd(__m256 a, __m256 b, __m256 c) noexcept {
#ifdef __FMA__
        return _mm256_fmadd_ps(a, b, c);  // If FMA is available
#else
        return _mm256_add_ps(_mm256_mul_ps(a, b), c);  // Fall back to separate multiply and add
#endif
    }

    // Fused multiply-subtract: (a * b) - c
    inline __m256 AVX_MultiplySubtract(__m256 a, __m256 b, __m256 c) noexcept {
#ifdef __FMA__
        return _mm256_fmsub_ps(a, b, c);  // If FMA is available
#else
        return _mm256_sub_ps(_mm256_mul_ps(a, b), c);  // Fall back to separate multiply and subtract
#endif
    }

    // Negated fused multiply-add: -(a * b) + c
    inline __m256 AVX_NegativeMultiplyAdd(__m256 a, __m256 b, __m256 c) noexcept {
#ifdef __FMA__
        return _mm256_fnmadd_ps(a, b, c);  // If FMA is available
#else
        return _mm256_add_ps(_mm256_sub_ps(_mm256_setzero_ps(), _mm256_mul_ps(a, b)), c);
#endif
    }

    /******************************
    *                             *
    *     AVX Dot Product Ops     *
    *                             *
    ******************************/

    // Computes the dot product of two vectors
    inline float AVX_Dot(__m256 a, __m256 b) noexcept {
        // Perform element-wise multiplication
        __m256 multResult = AVX_Multiply(a, b);

        // Horizontal add to sum all elements in the vectors
        __m128    lowHalf = _mm256_castps256_ps128(multResult);   // Low 128 bits
        __m128   highHalf = _mm256_extractf128_ps(multResult, 1); // High 128 bits
        __m128        sum = _mm_add_ps(lowHalf, highHalf);

        sum = _mm_hadd_ps(sum, sum);
        sum = _mm_hadd_ps(sum, sum);

        // Extract the scalar result
        return _mm_cvtss_f32(sum);
    }

    /******************************
    *                             *
    *     AVX Cross Product Ops   *
    *                             *
    ******************************/

    // Computes the cross product of two vectors
    inline __m256 AVX_Cross(__m256 a, __m256 b) noexcept {
        __m256     a_yzx = _mm256_permute_ps(a, _MM_SHUFFLE(3, 0, 2, 1));
        __m256     b_yzx = _mm256_permute_ps(b, _MM_SHUFFLE(3, 0, 2, 1));

        __m256 crossProd = AVX_Subtract(AVX_Multiply(a, b_yzx), AVX_Multiply(a_yzx, b));

        return _mm256_permute_ps(crossProd, _MM_SHUFFLE(3, 0, 2, 1));
    }

    /******************************
    *                             *
    *     AVX Helper Functions    *
    *                             *
    ******************************/

    // Extract the first element (index 0) from an __m256 vector
    inline float AVX_GetX(__m256 v) noexcept {
        return _mm_cvtss_f32(_mm256_castps256_ps128(v));
    }

    // Extract the second element (index 1) from an __m256 vector
    inline float AVX_GetY(__m256 v) noexcept {
        return _mm_cvtss_f32(_mm_shuffle_ps(_mm256_castps256_ps128(v), _mm256_castps256_ps128(v), _MM_SHUFFLE(1, 1, 1, 1)));
    }

    // Extract the third element (index 2) from an __m256 vector
    inline float AVX_GetZ(__m256 v) noexcept {
        return _mm_cvtss_f32(_mm_shuffle_ps(_mm256_castps256_ps128(v), _mm256_castps256_ps128(v), _MM_SHUFFLE(2, 2, 2, 2)));
    }

    // Extract the fourth element (index 3) from an __m256 vector
    inline float AVX_GetW(__m256 v) noexcept {
        return _mm_cvtss_f32(_mm_shuffle_ps(_mm256_castps256_ps128(v), _mm256_castps256_ps128(v), _MM_SHUFFLE(3, 3, 3, 3)));
    }

    /******************************
    *                             *
    *     AVX Utility Functions   *
    *                             *
    ******************************/

    // Computes the dot product for two large arrays
    inline float AVX_DotArrays(const float* a, const float* b, size_t count) noexcept {
        __m256 result = AVX_Set(0.0f);
        for (size_t i = 0; i < count; i += 8) {
            __m256 va = AVX_Load(&a[i]);
            __m256 vb = AVX_Load(&b[i]);
            result = AVX_Add(result, AVX_Multiply(va, vb));
        }
        return AVX_Dot(result, AVX_Set(1.0f)); // Sum all elements in result
    }
}

