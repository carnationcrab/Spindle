// TestFramework.h
#pragma once

#include <vector>
#include <functional>
#include <string>
#include <cmath>
#include "../Log.h"

namespace TestFramework {

    struct Test {
        std::string name;
        std::function<bool()> passed;
    };

    class TestRunner {
    public:
        static TestRunner& getInstance() {
            static TestRunner instance;
            return instance;
        }

        void addTest(const std::string& name, std::function<bool()> passed) {
            tests.push_back({ name, passed });
        }

        void runAllTests() {
            int passed{ 0 };
            int failed{ 0 };
            for (const auto& test : tests) {
                SPINDLE_TEST_INFO("Running test: {}...", test.name);
                bool result = test.passed();
                if (result) {
                    SPINDLE_TEST_PASS("{}: PASSED", test.name);
                    passed++;
                }
                else {
                    SPINDLE_TEST_FAIL("{}: FAILED", test.name);
                    failed++;
                }
            }
            SPINDLE_TEST_INFO("\nTotal tests run: {}", tests.size());
            SPINDLE_TEST_INFO("Passed: {}", passed);
            SPINDLE_TEST_INFO("Failed: {}", failed);
        }

    private:
        std::vector<Test> tests;
    };

    // different types of equality
    inline bool assertEqual(float a, float b, const std::string& testDescription, float epsilon = 1e-6f) {
        if (std::fabs(a - b) > epsilon) {
            SPINDLE_TEST_FAIL("Assertion failed: {} ({:.6f} != {:.6f})", testDescription, a, b);
            return false;
        }
        return true;
    }

    inline bool assertEqual(int a, int b, const std::string& testDescription) {
        if (a != b) {
            SPINDLE_TEST_FAIL("Assertion failed: {} ({} != {})", testDescription, a, b);
            return false;
        }
        return true;
    }

    inline bool assertEqual(const std::string& a, const std::string& b, const std::string& testDescription) {
        if (a != b) {
            SPINDLE_TEST_FAIL("Assertion failed: {} (\"{}\" != \"{}\")", testDescription, a, b);
            return false;
        }
        return true;
    }

    inline bool assertTrue(bool condition, const std::string& message) {
        if (!condition) {
            SPINDLE_TEST_FAIL("Assertion failed: {}", message);
            return false;
        }
        return true;
    }
}

#define TEST_CASE(name) \
    bool name(); \
    struct name##_Register { \
        name##_Register() { TestFramework::TestRunner::getInstance().addTest(#name, name); } \
    } name##_instance; \
    bool name()

#define RUN_ALL_TESTS() \
    TestFramework::TestRunner::getInstance().runAllTests()
