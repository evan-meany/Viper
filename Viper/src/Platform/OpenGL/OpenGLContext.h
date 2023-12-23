#pragma once

#include "Viper/Renderer/GraphicsContext.h"

// Foward declare window so we don't need to include
struct GLFWwindow;

namespace Viper {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};

}