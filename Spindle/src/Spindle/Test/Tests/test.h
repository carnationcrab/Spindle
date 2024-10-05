#pragma once
#include "../../Log.h"

namespace Spindle {
    class test {
    public:
        virtual void RunAllTests() = 0;

    // protected:
        bool isPass(float expected, float actual) {
            return std::abs(expected - actual) < 0.001; // Floating point tolerance
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

