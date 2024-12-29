#pragma once

// testing framework
#include "Test/SpindleTest.h"

// test files to be run 
// TODO: this is inelegant, could it be automated?
#include "Test/SettingsTests.cpp"
#include "Test/PointTests.cpp"
#include "Test/Vector2Tests.cpp"
#include "Test/Vector3Tests.cpp"
#include "Test/MatrixTests.cpp"
#include "Test/QuaternionTests.cpp"

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