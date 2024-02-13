#pragma once

#include "RenderCommand.h"
#include "Viper/Camera/Camera.h"
#include "Viper/Camera/OrthographicCamera.h"
#include "Shader.h"

namespace Viper {

	class Renderer
	{
	public:
		static void Init();
		static void BeginScene(Camera& camera);
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void Submit(const Shared<Shader>& shader,
						   const Shared<VertexArray>& vertexArray,
						   const glm::mat4& transform = glm::mat4(1.0f));
		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		static void OnWindowResize(uint32_t width, uint32_t height);
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};

}