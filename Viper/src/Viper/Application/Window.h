#pragma once

// Currently this file is in the Application folder, potentially move this

#include "Viper/Core/Core.h"
#include "Viper/Events/Event.h"

namespace Viper {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Viper Engine",
				    unsigned int width = 1200,
					unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class VIPER_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		
		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		// Expose window
		virtual void* GetNativeWindow() const = 0;

		// To be implemented per platform
		static Window* Create(const WindowProps& props = WindowProps());
	};

} // End namespace Viper