#include "vppch.h"
#include "OpenGLContext.h"

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
		VP_CORE_ASSERT(status, "Failed to initialize Glad!");

		VP_CORE_INFO("OpenGL Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		VP_CORE_INFO("OpenGL Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		VP_CORE_INFO("OpenGL Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

		int versionMajor, versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
		VP_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), 
					   "Viper requires at least OpenGL version 4.5");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}