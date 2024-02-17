#pragma once

#include "Viper/Renderer/Texture.h"
#include "Viper/Camera/OrthographicCamera.h"

namespace Viper {

	// This this going to be completely static
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, 
							 const glm::vec2& size,
							 const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position,
							 const glm::vec2& size,
							 const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, 
							 const glm::vec2& size,
							 const Texture2D& texture);
		static void DrawQuad(const glm::vec3& position,
							 const glm::vec2& size,
							 const Texture2D& texture);
		// TODO: add a draw for a texture and a color tint
		// TODO: could create a 'quad' struct that would be passed to the Renderer
	};

}
