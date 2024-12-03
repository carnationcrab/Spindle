#pragma once

#include "../SSE.h"

namespace Spindle {

	// check if a number is real
    inline bool isReal(float number) noexcept {
        __m128 num = SSE_Set(number);

        // Check if the number is ordered (not NaN)
        __m128 ordCheck = SSE_CmpOrd(num); // returns true if not NaN

        // Check if the number is finite (not infinity)
        __m128 absNum = _mm_and_ps(num, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF))); // absolute value
        // __m128 infCheck = _mm_cmplt_ss(absNum, _mm_set_ss(std::numeric_limits<float>::infinity()));

        //return _mm_comieq_ss(infCheck, _mm_set_ss(1.0f));
    }




}
