#pragma once

#include "Window.h"

#include "Viper/Core/Core.h"
#include "Viper/Core/Timestep.h"
#include "Viper/Events/Event.h"
#include "Viper/Events/ApplicationEvent.h"
#include "Viper/Layer/LayerStack.h"
#include "Viper/ImGui/ImGuiLayer.h"
#include "Viper/Input/JoystickManager.h"

namespace Viper
{
	class  Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		
		void OnEvent(Event& e);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Unique<Window> m_Window;
		Unique<JoystickManager> m_JoystickManager;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();

}
