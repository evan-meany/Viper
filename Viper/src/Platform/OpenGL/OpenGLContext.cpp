#include "vppch.h"
#include "OpenGLContext.h"
#include "Viper/Log.h"

#include <GLFW/glfw3.h>
#include <Glad/glad.h>

namespace Viper {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	{
		m_WindowHandle = windowHandle;
		VP_CORE_ASSERT(windowHandle, "Handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle); // passes the window to the renderer
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		VP_CORE_ASSERT(status, "Failed to initialize Glad!")
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}