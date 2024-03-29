#pragma once

#include "Viper/Application/Window.h"
#include "Viper/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Viper {

	/*
	Note that WindowsWindow currently assumes that there is only one window instance
	that exists for the application. If this were to ever change I would need to create
	some sort of manager to handle things like controllers separate those from the window
	*/
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline float AspectRatio() const override 
		{ 
			return static_cast<float>(m_Data.Width) / static_cast<float>(m_Data.Height); 
		}

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override 
		{ 
			m_Data.EventCallback = callback;
		}
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		// Expose window
		inline virtual void* GetNativeWindow() const { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width; 
			unsigned int Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};
		WindowData m_Data;
	};

}