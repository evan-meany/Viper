#include <Viper.h>

#include <imgui.h>

class ExampleLayer : public Viper::Layer
{
public:
	ExampleLayer() : Layer("Example") 
	{
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("EVAN");
		ImGui::Text("Wassup");
		ImGui::End();
	}

	void OnUpdate() override
	{
		// Polling example
		if (Viper::Input::IsKeyPressed(VP_KEY_LEFT_CONTROL))
		{
			VP_TRACE("CONTROL key pressed.");
		}		
		if (Viper::Input::IsKeyPressed(VP_KEY_F))
		{
			VP_TRACE("F key pressed.");
		}
	}

	void OnEvent(Viper::Event& evnt) override
	{
		// Event example
		if (evnt.GetEventType() == Viper::EventType::KeyPressed)
		{
			Viper::KeyPressedEvent& keyPressed = (Viper::KeyPressedEvent&)evnt;
			VP_TRACE("{0}", (char)keyPressed.GetKeyCode());
		}
	}
};

class Sandbox : public Viper::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
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