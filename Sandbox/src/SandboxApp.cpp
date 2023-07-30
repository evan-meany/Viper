#include <Viper.h>

class ExampleLayer : public Viper::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
		VP_INFO("ExampleLayer::Update");
	}

	void OnEvent(Viper::Event& evnt) override
	{
		VP_TRACE("{0}", evnt);
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