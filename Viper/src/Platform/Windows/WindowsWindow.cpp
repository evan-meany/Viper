#include "vppch.h"
#include "WindowsWindow.h"

#include "Viper/Events/ApplicationEvent.h"
#include "Viper/Events/MouseEvent.h"
#include "Viper/Events/KeyEvent.h"
#include "Viper/GLFW/GLFWManager.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Viper {

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{

	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		VP_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		// Initialize GLFW if not already initialized
		GLFWManager::Init();

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		
		// Setup renderer API and give window handle -- agnostic of API choice
		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data); // m_Data passed to callbacks
		SetVSync(true);

		// Setup GLFW event callbacks

		// WINDOW Events
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
			WindowResizeEvent resizeEvent(width, height);
			data.EventCallback(resizeEvent);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent closeEvent;
			data.EventCallback(closeEvent);
		});

		// KEYBOARD Events
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent pressedEvent(key, 0);
					data.EventCallback(pressedEvent);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent releasedEvent(key);
					data.EventCallback(releasedEvent);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent pressedEvent(key, 1);
					data.EventCallback(pressedEvent);
					break;
				}
				default:
				{
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int key)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent typedEvent(key);
			data.EventCallback(typedEvent);
		});
		
		// MOUSE Events
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent pressedEvent(button);
					data.EventCallback(pressedEvent);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent releasedEvent(button);
					data.EventCallback(releasedEvent);
					break;
				}
				default:
				{
					break;
				}
			}
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPosition, double yPosition)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent movedEvent((float)xPosition, (float)yPosition);
			data.EventCallback(movedEvent);
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent scrolledEvent((float)xOffset, (float)yOffset);
			data.EventCallback(scrolledEvent);
		});

	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled) { glfwSwapInterval(1); }
		else { glfwSwapInterval(0); }

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

} // End namespace Viper