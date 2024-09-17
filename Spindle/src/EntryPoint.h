#pragma once

#ifdef SPINDLE_PLATFORM_WINDOWS

extern Spindle::Application* Spindle::CreateApplication();

int main(int argc, char** argv)
{
	Spindle::Application* app = Spindle::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif