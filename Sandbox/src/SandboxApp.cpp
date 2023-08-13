#include <Viper.h>

class ExampleLayer : public Viper::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
	}

	void OnEvent(Viper::Event& evnt) override
	{
	}
};

class Sandbox : public Viper::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Viper::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Viper::Application* Viper::CreateApplication()
{
	return new Sandbox();
}