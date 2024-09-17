#include <Spindle/Spindle.h>

class Sandbox : public Spindle::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Spindle::Application* Spindle::CreateApplication()
{
	SPINDLE_INFO("Sandbox application created.");
	return new Sandbox();
}