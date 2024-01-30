#include "vppch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Viper {

	Shader* Shader::Create(const std::string& vertexSource, 
			     		   const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			VP_CORE_ASSERT(false, "RendererAPI cannot be None");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSource, fragmentSource);
		}
		
		VP_CORE_ASSERT(false, "OpenGLShader should not be nullptr");
		return nullptr;
	}

} // end Viper namespace