#include "vppch.h"
#include "GLFWManager.h"

#include <GLFW/glfw3.h>

namespace Viper {
	Shared<GLFWManager> GLFWManager::s_Instance = nullptr;

	static void GLFWErrorCallback(int error, const char* desciption)
	{
		VP_CORE_ERROR("GLFW Error {0}: {1}", error, desciption);
	}

	void GLFWManager::Init()
	{
		if (!s_Instance)
		{
			int success = glfwInit();
			VP_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_Instance = MakeShared<GLFWManager>();
		}
	}

	void GLFWManager::OnUpdate()
	{
		glfwPollEvents();
	}
}