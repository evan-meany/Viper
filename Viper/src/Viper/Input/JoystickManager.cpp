#include "vppch.h"
#include "JoystickManager.h"
#include "Platform/Windows/WindowsJoystickManager.h"

namespace Viper {

	Unique<JoystickManager> JoystickManager::s_Instance = nullptr;

	void JoystickManager::Create()
	{
#ifdef _WIN32
		s_Instance = std::make_unique<WindowsJoystickManager>();
		return;
#endif

		VP_CORE_ASSERT(false, "Operating System unknown");
	}

}
