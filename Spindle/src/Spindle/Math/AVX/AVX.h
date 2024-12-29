#pragma once

#include <immintrin.h> // AVX intrinsics
#include <string>
#include <sstream>

// AVX utilities for 256-bit SIMD operations
namespace Spindle {

    /******************************
    *   vector initialisation     *
    ******************************/

    // initializes an __m256 variable with eight float values
    inline __m256 AVX_Set(
        float x, 
        float y, 
        float z = 0.0f, 
        float w = 0.0f, 
        float a = 0.0f, 
        float b = 0.0f, 
        float c = 0.0f, 
        float d = 0.0f) noexcept {
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

    /******************************
    *         comparison          *
    ******************************/

    inline __m256 SSE_SetZero() {
        return _mm256_setzero_ps();
    }

    inline __m256 AVX_CompareEqual(__m256 a, __m256 b) {
        return _mm256_cmp_ps(a, b, _CMP_EQ_OQ);
    }

    inline __m256 AVX_CompareNotEqual(__m256 a, __m256 b) {
        return _mm256_cmp_ps(a, b, _CMP_NEQ_OQ);
    }

    inline bool AVX_AllTrue(__m256 cmp) {
        return _mm256_testc_ps(cmp, _mm256_set1_ps(-1.0f));
    }

    inline bool AVX_AnyTrue(__m256 cmp) {
        return !_mm256_testz_ps(cmp, _mm256_set1_ps(-1.0f));
    }


    /******************************
    *          utilities          *
    ******************************/

    inline float AVX_GetScalar(__m128 v) {
        return _mm_cvtss_f32(v);
    }

    // extracts the first element (index 0) from an __m256 vector
    inline float AVX_GetX(__m256 v) noexcept {
        return AVX_GetScalar(_mm256_castps256_ps128(v));
    }

    // extracts the second element (index 1) from an __m256 vector
    inline float AVX_GetY(__m256 v) noexcept {
        return AVX_GetScalar(_mm_shuffle_ps(_mm256_castps256_ps128(v), _mm256_castps256_ps128(v), _MM_SHUFFLE(1, 1, 1, 1)));
    }

    // extracts the third element (index 2) from an __m256 vector
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

    // extracts the last 128 bits of a 256-bit AVX vector.
    inline __m128 AVX_GetLast128(__m256 v) {
        return _mm256_extractf128_ps(v, 1);
    }

    // swaps the high and low halves of a 256-bit AVX vector.
    inline __m256 AVX_SwapHalves(__m256 v) {
        return _mm256_permute2f128_ps(v, v, 1);
    }

    inline __m256 AVX_ShuffleYZXW(__m256 v) noexcept {
        return _mm256_permute_ps(v, _MM_SHUFFLE(3, 0, 2, 1));
    }

    inline std::string AVX_ToString(__m256 vec) noexcept {
        alignas(32) float elements[8];
        _mm256_store_ps(elements, vec); // Store the contents of the AVX register into the array

        std::ostringstream oss;
        oss << "(";
        for (int i = 0; i < 8; ++i) {
            oss << elements[i];
            if (i < 7) {
                oss << ", ";
            }
        }
        oss << ")";
        return oss.str();
    }

    /******************************
    *          methods            *
    ******************************/

    // addition
    // TODO this is the same as SSE Add. SSE and AVX should likely
    // be combined into one SIMD thing.
    inline __m128 AVX_Add(__m128 a, __m128 b) noexcept {
        return _mm_add_ps(a, b);
    }

    inline __m256 AVX_Add(__m256 a, __m256 b) noexcept {
        return _mm256_add_ps(a, b);
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

    inline __m256 AVX_HorizontalAdd(__m256 v) {
        // shuffle and add the two 128-bit halves
        __m256 swappedV = AVX_SwapHalves(v);      // swap high and low halves
        __m256     sum1 = AVX_Add(v, swappedV);   // add together

        // perform horizontal addition within each 128-bit lane
        __m256   sum2 = _mm256_hadd_ps(sum1, sum1); // horizontal add
        __m256 result = _mm256_hadd_ps(sum2, sum2); // twice

        return result;
    }

    // computes the cross product of two vectors
    inline __m256 AVX_Cross(__m256 a, __m256 b) noexcept {
        __m256     a_yzx = _mm256_permute_ps(a, _MM_SHUFFLE(3, 0, 2, 1));
        __m256     b_yzx = _mm256_permute_ps(b, _MM_SHUFFLE(3, 0, 2, 1));

        __m256 crossProd = AVX_Subtract(AVX_Multiply(a, b_yzx), AVX_Multiply(a_yzx, b));

        return _mm256_permute_ps(crossProd, _MM_SHUFFLE(3, 0, 2, 1));
    }

    // computes the dot product of two vectors
    inline float AVX_Dot(__m256 a, __m256 b) noexcept {
        __m256 multResult = AVX_Multiply(a, b);

        __m128    lowHalf = AVX_GetFirst128(multResult);   // low 128 bits
        __m128   highHalf = AVX_GetLast128(multResult);    // high 128 bits
        __m128        sum = AVX_Add(lowHalf, highHalf);

        sum = AVX_HorizontalAdd(sum, sum);
        sum = AVX_HorizontalAdd(sum, sum);

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
        return AVX_Dot(result, AVX_Set(1.0f)); // sum all elements in result
    }

}

 