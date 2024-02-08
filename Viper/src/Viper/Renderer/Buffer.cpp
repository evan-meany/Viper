#include "vppch.h"
#include "Buffer.h"
#include "Viper/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Viper {

	Shared<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: 
			VP_CORE_ASSERT(false, "RendererAPI cannot be None");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		VP_CORE_ASSERT(false, "VertexBuffer should not be nullptr");
		return nullptr;
	}

	Shared<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			VP_CORE_ASSERT(false, "RendererAPI cannot be None");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(indices, size);
		}

		VP_CORE_ASSERT(false, "IndexBuffer should not be nullptr");
		return nullptr;
	}
}