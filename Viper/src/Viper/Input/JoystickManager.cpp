#include "vppch.h"
#include "JoystickManager.h"
#include "Platform/Windows/WindowsJoystickManager.h"

namespace Viper {

	Unique<JoystickManager> JoystickManager::s_Instance = nullptr;

	void JoystickManager::Create()
	{
#ifdef VP_PLATFORM_WINDOWS
		s_Instance = std::make_unique<WindowsJoystickManager>();
		return;
#endif

		VP_CORE_ASSERT(false, "Operating System unknown");
	}

}
