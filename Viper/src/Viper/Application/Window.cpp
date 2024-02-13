#include "vppch.h"
#include "Window.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Viper {

	Unique<Window> Window::Create(const WindowProps& props)
	{
#ifdef VP_PLATFORM_WINDOWS
		return std::make_unique<WindowsWindow>(props);
#endif

		VP_CORE_ASSERT(false, "Operating System unknown");
		return nullptr;
	}

}
