#include "vppch.h"
#include "Texture.h"
#include "Viper/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Viper {

	Shared<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			VP_CORE_ASSERT(false, "RendererAPI cannot be None");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return MakeShared<OpenGLTexture2D>(width, height);
		}

		VP_CORE_ASSERT(false, "Texture2D should not be nullptr");
		return nullptr;
	}

	Shared<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			VP_CORE_ASSERT(false, "RendererAPI cannot be None");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return MakeShared<OpenGLTexture2D>(path);
		}

		VP_CORE_ASSERT(false, "Texture2D should not be nullptr");
		return nullptr;
	}

}