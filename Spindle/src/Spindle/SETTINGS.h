#pragma once

/***********************
*                      *
*      SETTINGS        *
*                      *
***********************/

#define USE_AVX
// SIMD configuration
#if defined(__AVX__) || defined(__AVX2__)
#define USE_AVX
#elif defined(__SSE4_2__) || defined(__SSE2__)
#define USE_SSE
#else
#define USE_SCALAR
#endif

///////////////////////////////////////////////////////////

/***********************
*                      *
*        TO DO         *
*                      *
***********************/

// Platform Configuration, maybe

//#if defined(_WIN32) || defined(_WIN64)
//#define PLATFORM_WINDOWS
//#elif defined(__linux__)
//#define PLATFORM_LINUX
//#elif defined(__APPLE__)
//#define PLATFORM_MAC
//#else
//#error "Unsupported platform"
//#endif

// feature flags
 
//#define ENABLE_PROFILING    // runtime profiling
//#define ENABLE_PHYSICS      // physics systems
//#define ENABLE_RENDERER     // rendering systems
//#define ENABLE_AUDIO        // audio systems
