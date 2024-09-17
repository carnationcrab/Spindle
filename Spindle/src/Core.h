#pragma once

#ifdef SPINDLE_PLATFORM_WINDOWS
	#ifdef SPINDLE_BUILD_DLL
		#define SPINDLE_API __declspec(dllexport)
	#else
		#define SPINDLE_API __declspec(dllimport)
	#endif
#else
	#error SORRY: Spindle only supports Windows for now :(
#endif