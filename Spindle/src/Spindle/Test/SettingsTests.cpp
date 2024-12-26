#pragma once

#include "SpindleTest.h"
#include <string>

using namespace Spindle;

TEST_CASE(Settings_SIMD) {
#ifdef USE_AVX
    SpindleTest::assertInfo("App using AVX Intrinsics");
#elif defined(USE_SSE)
    SpindleTest::assertInfo("App using SSE Intrinsics");
#else
    SpindleTest::assertInfo("App not using Intrinsics");
#endif
   
}
