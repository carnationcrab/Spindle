#pragma once
#define _USE_MATH_DEFINES

// testing framework
#include "Test/SpindleTest.h"

// test files to be run 
// TODO: this is inelegant, could it be automated?
#include "Test/SettingsTests.cpp"
#include "Test/PointTests.cpp"
#include "Test/VectorTests.cpp"
#include "Test/MatrixTests.cpp"
#include "Test/QuaternionTests.cpp"
#include "Test/LineTests.cpp"
#include "Test/LineSegmentTests.cpp"
#include "Test/RayTests.cpp"
#include "Test/SphereTests.cpp"

#ifdef SPINDLE_PLATFORM_WINDOWS

extern Spindle::Application* Spindle::CreateApplication();

int main(int argc, char** argv)
{
	Spindle::Log::Init();

	SPINDLE_CORE_WARN("Log Initialized.");
	SPINDLE_INFO("Spinning up Spindle...");


	#ifdef SPINDLE_TEST
		Spindle::Initialiser::Init();

		SPINDLE_CORE_WARN("Initializing in TEST MODE...");
		RUN_ALL_TESTS();
	#else
		SPINDLE_CORE_WARN("Initializing in PRODUCTION MODE...");
		Spindle::Application* app = Spindle::CreateApplication();
		app->Run();
		delete app;
	#endif

	return 0;
}

#endif