#pragma once

#include "Viper/Renderer/RendererAPI.h"

namespace Viper {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Shared<VertexArray>& vertexArray) override;
	};

}