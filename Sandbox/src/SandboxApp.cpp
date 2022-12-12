#include <Viper.h>

class Sandbox : public Viper::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Viper::Application* Viper::CreateApplication()
{
	return new Sandbox();
}