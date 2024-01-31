#include "vppch.h"
#include "Application.h"
#include "Input.h"
#include "Events/Event.h"
#include "Renderer/Renderer.h"

#include <glfw/glfw3.h>

namespace Viper {
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		VP_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(VP_BIND_EVENT_FUNC(Application::OnEvent));
		//m_Window->SetVSync(false);

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
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(VP_BIND_EVENT_FUNC(Application::OnWindowClose));

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

	bool Application::OnWindowClose(WindowCloseEvent& evnt)
	{
		m_Running = false;
		return true;
	}

}