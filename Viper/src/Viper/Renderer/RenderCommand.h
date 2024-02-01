#pragma once

#include "RendererAPI.h"

namespace Viper {

	class RenderCommand
	{
	public:
		static void SetClearColor(glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		static void Clear()
		{
			s_RendererAPI->Clear();
		}
		static void DrawIndexed(const Shared<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};

}