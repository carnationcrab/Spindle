#include "tester.h"
#include "Tests/mathsTest.h"
#include "../Log.h"

namespace Spindle {

	void tester::runMathsTests() {
		SPINDLE_CORE_INFO("[tester] Starting maths tests...");
		
		mathsTest mathsTests;
		mathsTests.runAllTests();

		SPINDLE_CORE_INFO("[tester] Finished maths tests.");
	}

	void tester::runAllTests() {
		SPINDLE_CORE_INFO("[tester] Starting all tests tests...");
		// run maths tests
		runMathsTests();
	}

}
