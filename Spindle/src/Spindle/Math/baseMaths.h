#pragma once

#include <cmath>

//_ BASIC MATHS FNS _____________

namespace Spindle {
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

    // sine
    template <typename T>
    constexpr T SIN(const T angle) {
        return std::sin(angle);
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

    // min (must override system macros so ours work)
    #undef MIN
    template <typename T, typename T2>
    constexpr auto MIN(const T a, const T2 b) {
	    return a < b ? a : b;
    }

    // max
    #undef MAX
    template <typename T, typename T2>
    constexpr auto MAX(const T a, const T2 b) {
	    return a > b ? a : b;
    }
}
