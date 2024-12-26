#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <functional>
#include <vector>
#include <iostream>
#include <algorithm>
#include "../Log.h"

namespace SpindleTest {

    using TestFunction = std::function<void()>;

    class TestFramework {
    public:
        static TestFramework& getInstance() {
            static TestFramework instance;
            return instance;
        }

        // add a new test case
        void registerTest(const std::string& testName, TestFunction testFn) {
            if (tests.find(testName) != tests.end()) {
                SPINDLE_TEST_FAIL("Test '{}' is already registered. Ignoring duplicate registration.", testName);
                return;
            }
            // move semantics
            tests[testName] = std::move(testFn);
        }

        // run all tests
        void runAllTests() {
            size_t passed = 0;
            size_t failed = 0;

            SPINDLE_TEST_INFO("Spinning all tests...");

            // sort tests by their categories (prefix before '_')
            // this is the worst, I know
            std::vector<std::pair<std::string, TestFunction>> sortedTests(tests.begin(), tests.end());
            std::sort(sortedTests.begin(), sortedTests.end(),
                [](const auto& a, const auto& b) { return getCategory(a.first) < getCategory(b.first); });

            std::string lastCategory;

            for (const auto& [testName, testFn] : sortedTests) {
                const std::string currentCategory = getCategory(testName);

                // log the start of a new test category
                if (currentCategory != lastCategory) {
                    SPINDLE_TEST_HEADER("=== Starting tests for category: {} ===", currentCategory);
                    lastCategory = currentCategory;
                }

                if (runSingleTest(testName, testFn)) {
                    ++passed;
                }
                else {
                    ++failed;
                }
            }
            logTestSummary(passed, failed);
        }

        // run a specific test by name
        void runTest(const std::string& testName) {
            auto it = tests.find(testName);
            if (it != tests.end()) {
                SPINDLE_TEST_INFO("Running test '{}'...", testName);
                if (runSingleTest(testName, it->second)) {
                    logTestSummary(1, 0);
                }
                else {
                    logTestSummary(0, 1);
                }
            }
            else {
                SPINDLE_TEST_FAIL("Test '{}' not found.", testName);
            }
        }

    private:
        std::unordered_map<std::string, TestFunction> tests;

        TestFramework() = default;

        bool runSingleTest(const std::string& testName, const TestFunction& testFn) {
            try {
                testFn();
                SPINDLE_TEST_PASS("{}: PASSED", testName);
                return true;
            }
            catch (const std::exception& e) {
                SPINDLE_TEST_FAIL("{}: FAILED with exception: {}", testName, e.what());
            }
            catch (...) {
                SPINDLE_TEST_FAIL("{}: FAILED with unknown error.", testName);
            }
            return false;
        }

        static std::string getCategory(const std::string& testName) {
            size_t pos = testName.find('_');
            return (pos != std::string::npos) ? testName.substr(0, pos) : "General";
        }

        void logTestSummary(size_t passed, size_t failed) const {
            size_t total = passed + failed;
            SPINDLE_TEST_INFO("Summary:");
            SPINDLE_TEST_INFO("  Total tests run: {}", total);
            SPINDLE_TEST_INFO("  Passed: {}", passed);
            failed > 0 ? SPINDLE_TEST_FAIL("  Failed: {}", failed) :
                         SPINDLE_TEST_INFO("  Failed: {}", failed);
        }
    };

    // assertions
    inline void assertInfo(const std::string& message) {
        SPINDLE_TEST_INFO(message);
    }

    inline void assertTrue(bool condition, const std::string& message) {
        if (!condition) {
            SPINDLE_TEST_FAIL("Assertion failed: {}", message);
            throw std::runtime_error(message);
        }
    }

    inline void assertEqual(float a, float b, const std::string& message, float epsilon = 1e-6f) {
        if (std::fabs(a - b) > epsilon) {
            SPINDLE_TEST_FAIL("Assertion failed: {} ({} != {} within epsilon {})", message, a, b, epsilon);
            throw std::runtime_error(message);
        }
    }

    inline void assertEqual(int a, int b, const std::string& message) {
        if (a != b) {
            SPINDLE_TEST_FAIL("Assertion failed: {} ({} != {})", message, a, b);
            throw std::runtime_error(message);
        }
    }

    inline void assertEqual(const std::string& a, const std::string& b, const std::string& message) {
        if (a != b) {
            SPINDLE_TEST_FAIL("Assertion failed: {} (\"{}\" != \"{}\")", message, a, b);
            throw std::runtime_error(message);
        }
    }
}

// macros for test registration and execution
#define TEST_CASE(name) \
    void name(); \
    struct name##_Register { \
        name##_Register() { \
            SpindleTest::TestFramework::getInstance().registerTest(#name, name); \
        } \
    } name##_Register_Instance; \
    void name()

#define RUN_ALL_TESTS() \
    SpindleTest::TestFramework::getInstance().runAllTests()

#define RUN_TEST(name) \
    SpindleTest::TestFramework::getInstance().runTest(name)
