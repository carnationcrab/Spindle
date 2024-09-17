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
	return new Sandbox();
}