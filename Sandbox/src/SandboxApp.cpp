#include "ExampleLayer.h"
#include "Sandbox2D.h"

#include <Viper/Core/EntryPoint.h>

class Sandbox : public Viper::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer(GetWindow().AspectRatio()));
		PushLayer(new Sandbox2D());
		// ImGuiLayer automatically added
	}

	~Sandbox()
	{

	}
};

Viper::Application* Viper::CreateApplication()
{
	return new Sandbox();
}