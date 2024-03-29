#pragma once

#include "Viper/Renderer/RendererAPI.h"

namespace Viper {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetClearColor(glm::vec4& color) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Shared<VertexArray>& vertexArray) override;
	};

}