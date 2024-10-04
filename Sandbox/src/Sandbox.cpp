#include <Spindle/Spindle.h>

class Sandbox : public Spindle::Application
{
public:
	Sandbox()
	{

	}

	virtual ~Sandbox()
	{

	}
};

Spindle::Application* Spindle::CreateApplication()
{
	SPINDLE_INFO("Sandbox application created.");

	// no memory leak because destroyed on program end
	return new Sandbox();
}