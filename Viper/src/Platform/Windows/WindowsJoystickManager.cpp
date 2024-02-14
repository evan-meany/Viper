#include "vppch.h"
#include "WindowsJoystickManager.h"

#include "Viper/GLFW/GLFWManager.h"
#include "Viper/Events/JoystickEvent.h"

#include <GLFW/glfw3.h>

namespace Viper {
	Unique<WindowsJoystickManager::ControllerData> WindowsJoystickManager::s_Data = nullptr;

	WindowsJoystickManager::WindowsJoystickManager()
	{
		// Initialize GLFW if not already initialized
		GLFWManager::Init();

		if (!s_Data) { s_Data = std::make_unique<ControllerData>(); }
		
		s_Data->ConnectedIDs.reserve(GLFW_JOYSTICK_LAST);
		for (int jid = GLFW_JOYSTICK_1; jid < GLFW_JOYSTICK_LAST; jid++)
		{
			if (glfwJoystickPresent(jid) == GLFW_TRUE)
			{
				const char* name = glfwGetJoystickName(jid);
				VP_CORE_TRACE("Joystick {0} is present", name);
				s_Data->ConnectedIDs.push_back(jid);
				glfwSetJoystickUserPointer(jid, &s_Data);
			}
		}

		glfwSetJoystickCallback([](int jid, int e)
		{
			if (e == GLFW_CONNECTED)
			{
				s_Data->ConnectedIDs.push_back(jid);
				VP_CORE_TRACE("Joystick {0} connected", jid);
				JoystickConnectionEvent connectionEvent(true);
				s_Data->EventCallback(connectionEvent);
			}
			else if (e == GLFW_DISCONNECTED)
			{
				auto& ids = s_Data->ConnectedIDs;
				ids.erase(std::remove(ids.begin(), ids.end(), jid), ids.end());
				VP_CORE_TRACE("Joystick {0} disconnected", jid);
				JoystickConnectionEvent connectionEvent(false);
				s_Data->EventCallback(connectionEvent);
			}
		});
	}

	WindowsJoystickManager::~WindowsJoystickManager()
	{
	}


	int WindowsJoystickManager::GetPrimaryPlayerIDImpl()
	{
		// If no controllers connected return -1
		if (s_Data->ConnectedIDs.empty()) { return -1; }
		return s_Data->ConnectedIDs.front();
	}

}