#include <spindle.h>

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
	SPINDLE_INFO("Sandbox application started.");
	return new Sandbox();
}