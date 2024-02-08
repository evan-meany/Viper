#include "vppch.h"
#include "VertexArray.h"
#include "Viper/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Viper {
	Shared<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			VP_CORE_ASSERT(false, "RendererAPI cannot be None");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		}

		VP_CORE_ASSERT(false, "VertexArray should not be nullptr");
		return nullptr;
	}
}