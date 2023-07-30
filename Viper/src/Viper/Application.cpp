#include "vppch.h"
#include "Application.h"

#include "Events/Event.h"
#include "Viper/Log.h"

#include <Glad/glad.h>

namespace Viper
{
#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		VP_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNC(OnEvent));

		unsigned int id;
		glGenVertexArrays(1, &id);
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			// Reset color of screen
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			// Render layers from back to front
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			// Poll for and handle events
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& evnt)
	{
		EventDispatcher dispatcher(evnt);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnWindowClose));

		VP_CORE_INFO("{0}", evnt);

		// Handle layer events from front to back
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(evnt);
			if (evnt.IsHandled()) { break; }
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