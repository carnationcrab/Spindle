#pragma once
#include "../../Log.h"

namespace Spindle {
    class test {
    public:
        virtual void runAllTests() = 0;

    // protected:
        bool isPass(float expected, float actual) {
            return std::abs(expected - actual) < 0.001; // Tolerance for floating-point comparison
        }

        void runTest(float expected, float actual, const std::string& description) {
            if (!isPass(expected, actual)) {
                SPINDLE_TEST_FAIL("{} FAILED: Expected {}, got {}", description, expected, actual);
            }
            else {
                SPINDLE_TEST_PASS("{} PASSED: Expected {}, got {}", description, expected, actual);
            }
        }
    };
}

