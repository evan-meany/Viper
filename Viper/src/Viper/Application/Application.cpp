#include "vppch.h"
#include "Application.h"

#include "Viper/Input/Input.h"
#include "Viper/Events/Event.h"
#include "Viper/Renderer/Renderer.h"
#include "Viper/GLFW/GLFWManager.h"

#include <glfw/glfw3.h>

namespace Viper {
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		VP_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallback(VP_BIND_EVENT_FUNC(Application::OnEvent));

		JoystickManager::Create();
		JoystickManager::SetEventCallback(VP_BIND_EVENT_FUNC(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		// Layers are deleted in LayerStack destructor
	}

	void Application::Run()
	{
		while (m_Running)
		{
			// Calculate delta time
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			// Render layers from back to front
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(timestep);
			}

			// Render the ImGui layers
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			// Poll for and handle events
			GLFWManager::OnUpdate();
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(VP_BIND_EVENT_FUNC(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(VP_BIND_EVENT_FUNC(Application::OnWindowResize));

		// Handle layer events from front to back
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.IsHandled()) { break; }
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushLayer(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0.0f || e.GetHeight() == 0.0f) 
		{ 
			m_Minimized = true;
			return false; 
		}
		m_Minimized = false;

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return true;
	}

}