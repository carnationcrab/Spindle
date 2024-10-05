#include "Tester.h"
#include "Tests/mathsTest.h"
#include "../Log.h"

namespace Spindle {

	void Tester::RunAllTests() {
		SPINDLE_TEST_INFO("[Tester] Starting all tests...");
		// run maths tests
		SPINDLE_TEST_INFO("[Tester] Starting maths tests...");

		mathsTest mathsTests;
		mathsTests.RunAllTests();

		SPINDLE_TEST_INFO("[Tester] Finished maths tests.");
	}

}
