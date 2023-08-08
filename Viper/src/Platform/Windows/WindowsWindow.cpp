#include "vppch.h"
#include "WindowsWindow.h"

#include "Viper/Log.h"
#include "Viper/Events/ApplicationEvent.h"
#include "Viper/Events/MouseEvent.h"
#include "Viper/Events/KeyEvent.h"

#include <glad/glad.h>

namespace Viper {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* desciption)
	{
		VP_CORE_ERROR("GLFW Error {0}: {1}", error, desciption);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

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

		if (!s_GLFWInitialized)
		{
			//TODO : glfwTerminate on system shutdown
			int success = glfwInit();
			VP_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		VP_CORE_ASSERT(status, "Failed to initialize Glad!")
		glfwSetWindowUserPointer(m_Window, &m_Data); // m_Data passed to callbacks
		SetVSync(true);

		// Setup GLFW event callbacks
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
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
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