#pragma once

#include <cmath>
#include <cassert>
#include <optional>

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
}

