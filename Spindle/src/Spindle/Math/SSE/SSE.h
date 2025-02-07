#pragma once

#include <xmmintrin.h>
#include <pmmintrin.h> // for horizontal add

// overcommented for my own learning purposes, sorry readers.

namespace Spindle {


    /******************************
    *   vector initialisation     *
    ******************************/

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


    /******************************
    *          utilities          *
    ******************************/

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

    // extracts the second element (index 2) from 
    // an __m128 vector
    inline float SSE_GetZ(__m128 v) noexcept {
        return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)));
    }

    // extracts the second element (index 3) from 
    // an __m128 vector
    inline float SSE_GetW(__m128 v) noexcept {
        return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3)));
    }

    inline __m128 SSE_ShuffleYZXW(__m128 v) noexcept {
        return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1));
    }

    inline __m128 SSE_ShuffleZXYW(__m128 v) noexcept {
        return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 1, 0, 2));
    }


    /******************************
    *         comparison          *
    ******************************/

    inline __m128 SSE_Min(__m128 a, __m128 b) {
        return _mm_min_ps(a, b);
    }

    inline __m128 SSE_Max(__m128 a, __m128 b) {
        return _mm_max_ps(a, b);
    }

    inline bool SSE_AllEqual(__m128 cmp) {
        return _mm_movemask_ps(cmp) == 0xF;
    }

    inline __m128 SSE_CompareEqual(__m128 a, __m128 b) {
        return _mm_cmpeq_ps(a, b);
    }

    inline __m128 SSE_CompareNotEqual(__m128 a, __m128 b) {
        return _mm_cmpneq_ps(a, b);
    }

    inline __m128 SSE_CompareNaN(__m128 a) {
        return _mm_cmpunord_ps(a, a); // Returns true if NaN
    }

    // compare less equal
    inline __m128 SSE_CompareLessEqual(__m128 a, __m128 b) {
        return _mm_cmple_ps(a, b);
    }

    inline bool SSE_IsLessEqual(__m128 a, __m128 b) {
        __m128 cmpResult = _mm_cmple_ps(a, b);
        return _mm_movemask_ps(cmpResult) != 0;
    }

    // compare greater equal
    inline __m128 SSE_CompareGreaterEqual(__m128 a, __m128 b) {
        return _mm_cmpge_ps(a, b);
    }

    inline __m128 SSE_CompareOrder(__m128 a) noexcept {
        return _mm_cmpord_ss(a, a);
    }

    inline __m128 SSE_CompareOrder(__m128 a, __m128 b) noexcept {
        return _mm_cmpord_ss(a, b);
    }

    inline bool SSE_AllTrue(__m128 mask) {
        return _mm_movemask_ps(mask) == 0xF;
    }

    inline bool SSE_AnyTrue(__m128 mask) {
        return _mm_movemask_ps(mask) != 0x0;
    }

    // logical AND
    inline __m128 SSE_And(__m128 a, __m128 b) {
        return _mm_and_ps(a, b);
    }

    // logical OR
    inline __m128 SSE_Or(__m128 a, __m128 b) {
        return _mm_or_ps(a, b);
    }

    inline __m128 SSE_AndOr(__m128 andMask, __m128 orMask, __m128 input) {
        __m128 andResult = _mm_and_ps(andMask, input);
        return _mm_or_ps(andResult, orMask); 
    }

    inline __m128 SSE_AndNot(__m128 notMask, __m128 input) {
        return _mm_andnot_ps(notMask, input);
    }

    /******************************
    *          methods            *
    ******************************/

    // addition
    inline __m128 SSE_Add(__m128 a, __m128 b) noexcept {
        return _mm_add_ps(a, b);
    }

    // subtraction
    inline __m128 SSE_Subtract(__m128 a, __m128 b) noexcept {
        return _mm_sub_ps(a, b);
    }

    // multiplication
    inline __m128 SSE_Multiply(__m128 vec, float scalar) noexcept {
        __m128 scalarVec = _mm_set1_ps(scalar);           // Broadcast scalar across all SIMD lanes
        return _mm_mul_ps(vec, scalarVec);                // Perform SIMD multiplication
    }

    inline __m128 SSE_Multiply(__m128 a, __m128 b) noexcept {
        return _mm_mul_ps(a, b);
    }

    // fused multiply-add: (a * b) + c
    inline __m128 SSE_MultiplyAdd(__m128 a, __m128 b, __m128 c) noexcept {
#ifdef __FMA__
        return _mm_fmadd_ps(a, b, c); 
#else
        return _mm_add_ps(_mm_mul_ps(a, b), c);
#endif
    }

    // fused multiply-subtract: (a * b) - c
    inline __m128 SSE_MultiplySubtract(__m128 a, __m128 b, __m128 c) noexcept {
#ifdef __FMA__
        return _mm_fmsub_ps(a, b, c);
#else
        return _mm_sub_ps(_mm_mul_ps(a, b), c);
#endif
    }

    // negated fused multiply-add: -(a * b) + c
    inline __m128 SSE_NegativeMultiplyAdd(__m128 a, __m128 b, __m128 c) noexcept {
#ifdef __FMA__
        return _mm_fnmadd_ps(a, b, c);
#else
        return _mm_add_ps(_mm_sub_ps(_mm_setzero_ps(), _mm_mul_ps(a, b)), c);
#endif
    }

    inline __m128 SSE_HorizontalAdd(__m128 a, __m128 b) noexcept {
        return _mm_hadd_ps(a, b);
    }

    inline float SSE_Dot(__m128 a, __m128 b) noexcept {
        // perform element-wise multiplication
        __m128 multResult = SSE_Multiply(a, b);

        // horizontal add to sum the elements
        __m128  addResult = SSE_HorizontalAdd(multResult, multResult);
        addResult = SSE_HorizontalAdd(addResult, addResult);

        // extract the final result
        return SSE_GetX(addResult);
    }

    // computes the dot product using transposed loading approach
    // for large data
    inline __m128 SSE_Dot(
        const float* a, 
        const float* b) 
        noexcept {
        // load the vectors in transposed order
        __m128    vaX = SSE_Load(&a[0]);  // a[0, 4, 8, 12]
        __m128    vaY = SSE_Load(&a[1]);  // a[1, 5, 9, 13]
        __m128    vaZ = SSE_Load(&a[2]);  // a[2, 6, 10, 14]
        __m128    vaW = SSE_Load(&a[3]);  // a[3, 7, 11, 15]

        __m128    vbX = SSE_Load(&b[0]);  // b[0, 4, 8, 12]
        __m128    vbY = SSE_Load(&b[1]);  // b[1, 5, 9, 13]
        __m128    vbZ = SSE_Load(&b[2]);  // b[2, 6, 10, 14]
        __m128    vbW = SSE_Load(&b[3]);  // b[3, 7, 11, 15]

        // perform element-wise multiplication and accumulate the results
        __m128 result = SSE_Multiply(vaX, vbX);
               result = SSE_Add(result, SSE_Multiply(vaY, vbY));
               result = SSE_Add(result, SSE_Multiply(vaZ, vbZ));
               result = SSE_Add(result, SSE_Multiply(vaW, vbW));

        return result;
    }

    inline __m128 SSE_Dot(
        int count,
        float r[],
        const float a[],
        const float b[])
        noexcept {
        for (int i = 0; i < count; i += 4)
        {
            __m128    vaX = SSE_Load(&a[(i + 0) * 4]); // a[0,4,8,12]
            __m128    vaY = SSE_Load(&a[(i + 1) * 4]); // a[1,5,9,13]
            __m128    vaZ = SSE_Load(&a[(i + 2) * 4]); // a[2,6,10,14]
            __m128    vaW = SSE_Load(&a[(i + 3) * 4]); // a[3,7,11,15]
                      
            __m128    vbX = SSE_Load(&b[(i + 0) * 4]); // b[0,4,8,12]
            __m128    vbY = SSE_Load(&b[(i + 1) * 4]); // b[1,5,9,13]
            __m128    vbZ = SSE_Load(&b[(i + 2) * 4]); // b[2,6,10,14]
            __m128    vbW = SSE_Load(&b[(i + 3) * 4]); // b[3,7,11,15]
            
            __m128 result;
                   result = SSE_Multiply(vaX, vbX);
                   result = SSE_Add(result, _mm_mul_ps(vaY, vbY));
                   result = SSE_Add(result, _mm_mul_ps(vaZ, vbZ));
                   result = SSE_Add(result, _mm_mul_ps(vaW, vbW));
            
            return result;
        }
    }

    // dot product of two vectors
    //inline float SSE_Dot(__m128 a, __m128 b) noexcept {
    //    // perform element-wise multiplication
    //    __m128 multResult = SSE_Multiply(a, b);
    //     float  addResult = SSE_GetX(multResult) 
    //                        + SSE_GetY(multResult) 
    //                        + SSE_GetZ(multResult);
    //     return addResult;
    //}

    inline __m128 SSE_Cross(__m128 a, __m128 b) noexcept {
        __m128     a_yzx = SSE_ShuffleYZXW(a);
        __m128     b_yzx = SSE_ShuffleYZXW(a);

        __m128 crossProd = SSE_Subtract(SSE_Multiply(a, b_yzx), 
                                      SSE_Multiply(a_yzx, b));

        return SSE_ShuffleYZXW(crossProd);
    }
}

