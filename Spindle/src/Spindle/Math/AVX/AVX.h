#pragma once

#include <immintrin.h> // AVX intrinsics

// AVX utilities for 256-bit SIMD operations
namespace Spindle {

    /******************************
    *   vector initialisation     *
    ******************************/

    // initializes an __m256 variable with eight float values
    inline __m256 AVX_Set(float x, float y, float z, float w, float a, float b, float c, float d) noexcept {
        return _mm256_set_ps(d, c, b, a, w, z, y, x);
    }

    // broadcasts a single float across all lanes of an __m256 variable
    inline __m256 AVX_Set(float value) noexcept {
        return _mm256_set1_ps(value);
    }

    // loads eight floats from a 32-byte aligned array into an __m256 variable
    inline __m256 AVX_Load(const float* data) noexcept {
        return _mm256_load_ps(data);
    }

    // stores the contents of an __m256 variable into a 32-byte aligned array
    inline void AVX_Store(float* data, __m256 v) noexcept {
        _mm256_store_ps(data, v);
    }

    inline __m256 SSE_SetZero() {
        return _mm256_setzero_ps();
    }

    /******************************
    *          utilities          *
    ******************************/

    inline float AVX_GetScalar(__m128 v) {
        return _mm_cvtss_f32(v);
    }

    // extract the first element (index 0) from an __m256 vector
    inline float AVX_GetX(__m256 v) noexcept {
        return AVX_GetScalar(_mm256_castps256_ps128(v));
    }

    // extract the second element (index 1) from an __m256 vector
    inline float AVX_GetY(__m256 v) noexcept {
        return AVX_GetScalar(_mm_shuffle_ps(_mm256_castps256_ps128(v), _mm256_castps256_ps128(v), _MM_SHUFFLE(1, 1, 1, 1)));
    }

    // extract the third element (index 2) from an __m256 vector
    inline float AVX_GetZ(__m256 v) noexcept {
        return AVX_GetScalar(_mm_shuffle_ps(_mm256_castps256_ps128(v), _mm256_castps256_ps128(v), _MM_SHUFFLE(2, 2, 2, 2)));
    }

    // extract the fourth element (index 3) from an __m256 vector
    inline float AVX_GetW(__m256 v) noexcept {
        return AVX_GetScalar(_mm_shuffle_ps(_mm256_castps256_ps128(v), _mm256_castps256_ps128(v), _MM_SHUFFLE(3, 3, 3, 3)));
    }

    // extracts the first 128 bits of a 256-bit AVX vector.
    inline __m128 AVX_GetFirst128(__m256 v
    ) {
        return _mm256_castps256_ps128(v);
    }

    // Extracts the last 128 bits of a 256-bit AVX vector.
    inline __m128 AVX_GetLast128(__m256 v) {
        return _mm256_extractf128_ps(v, 1);
    }

    /******************************
    *         arithmatic          *
    ******************************/

    // addition
    inline __m256 AVX_Add(__m256 a, __m256 b) noexcept {
        return _mm256_add_ps(a, b);
    }

    inline __m128 AVX_Add(__m128 a, __m128 b) noexcept {
        return _mm_add_ps(a, b);
    }

    // subtraction
    inline __m256 AVX_Subtract(__m256 a, __m256 b) noexcept {
        return _mm256_sub_ps(a, b);
    }

    // multiplication
    inline __m256 AVX_Multiply(__m256 a, __m256 b) noexcept {
        return _mm256_mul_ps(a, b);
    }

    // multiplication against a scalar
    // (broadcasts the scalar to all lanes, then multiplies)
    inline __m256 AVX_Multiply(__m256 vec, float scalar) noexcept {
        __m256 scalarVec = AVX_Set(scalar);
        return _mm256_mul_ps(vec, scalarVec);
    }

    // fused multiply-add: (a * b) + c
    inline __m256 AVX_MultiplyAdd(__m256 a, __m256 b, __m256 c) noexcept {
#ifdef __FMA__
        return _mm256_fmadd_ps(a, b, c);
#else
        return AVX_Add(AVX_Multiply(a, b), c);
#endif
    }

    // fused multiply-subtract: (a * b) - c
    inline __m256 AVX_MultiplySubtract(__m256 a, __m256 b, __m256 c) noexcept {
#ifdef __FMA__
        return _mm256_fmsub_ps(a, b, c);
#else
        return AVX_Subtract(AVX_Multiply(a, b), c);
#endif
    }

    // negated fused multiply-add: -(a * b) + c
    inline __m256 AVX_NegativeMultiplyAdd(__m256 a, __m256 b, __m256 c) noexcept {
#ifdef __FMA__
        return _mm256_fnmadd_ps(a, b, c);
#else
        return AVX_Add(AVX_Subtract(SSE_SetZero(), AVX_Multiply(a, b)), c);
#endif
    }

    inline __m128 AVX_HorizontalAdd(__m128 a, __m128 b) noexcept {
        return _mm_hadd_ps(a, b);
    }


    /******************************
    *           methods           *
    ******************************/

    // Computes the dot product of two vectors
    inline float AVX_Dot(__m256 a, __m256 b) noexcept {
        // Perform element-wise multiplication
        __m256 multResult = AVX_Multiply(a, b);

        __m128    lowHalf = AVX_GetFirst128(multResult);   // low 128 bits
        __m128   highHalf = AVX_GetLast128(multResult);    // high 128 bits
        __m128        sum = AVX_Add(lowHalf, highHalf);

        // horizontal add the elements
        sum = AVX_HorizontalAdd(sum, sum);
        sum = AVX_HorizontalAdd(sum, sum);

        // extract the scalar result
        return AVX_GetScalar(sum);
    }

    // computes the dot product for two large arrays
    inline float AVX_Dot(const float* a, const float* b, size_t count) noexcept {
        __m256 result = AVX_Set(0.0f);
        for (size_t i = 0; i < count; i += 8) {
            __m256 va = AVX_Load(&a[i]);
            __m256 vb = AVX_Load(&b[i]);
           
               result = AVX_Add(result, AVX_Multiply(va, vb));
        }
        return AVX_Dot(result, AVX_Set(1.0f)); // Sum all elements in result
    }

    // Computes the cross product of two vectors
    inline __m256 AVX_Cross(__m256 a, __m256 b) noexcept {
        __m256     a_yzx = _mm256_permute_ps(a, _MM_SHUFFLE(3, 0, 2, 1));
        __m256     b_yzx = _mm256_permute_ps(b, _MM_SHUFFLE(3, 0, 2, 1));

        __m256 crossProd = AVX_Subtract(AVX_Multiply(a, b_yzx), AVX_Multiply(a_yzx, b));

        return _mm256_permute_ps(crossProd, _MM_SHUFFLE(3, 0, 2, 1));
    }
}

 