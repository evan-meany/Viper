#include "vppch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Viper {

	// The renderer uses a single shader so we only have to bind the shader once
	struct Renderer2DData
	{
		Shared<VertexArray> TextureVertexArray;
		Shared<Shader> TextureShader;
		// This is used for when there should be no texture applied to the draw
		Shared<Texture2D> WhiteTexture; 
	};
	static Unique<Renderer2DData> s_Data;

	void Renderer2D::Init()
	{
		s_Data = MakeUnique<Renderer2DData>();
		
		// Setup vertex array
		s_Data->TextureVertexArray = Viper::VertexArray::Create();
		float textureVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		auto textureVertexBuffer = Viper::VertexBuffer::Create(textureVertices, sizeof(textureVertices));
		textureVertexBuffer->SetLayout({ { Viper::ShaderDataType::Float3, "a_Position" },
										{ Viper::ShaderDataType::Float2, "a_TextureCoord" } });
		s_Data->TextureVertexArray->AddVertexBufffer(textureVertexBuffer);

		uint32_t textureIndices[6] = { 0, 1, 2, 2, 3, 0 };
		auto textureIndexBuffer = Viper::IndexBuffer::Create(textureIndices,
			sizeof(textureIndices) / sizeof(uint32_t));
		s_Data->TextureVertexArray->SetIndexBufffer(textureIndexBuffer);
		
		// Setup shader
		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");

		// Setup the empty white texture
		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));
	}

	void Renderer2D::Shutdown()
	{
		s_Data.release();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->UploadUniform("u_ViewProjection", camera.GetViewProjectionMatrix());
	}
	
	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position,
							  const glm::vec2& size,
							  const glm::vec4& color)
	{
		DrawQuad({position.x, position.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position,
							  const glm::vec2& size,
							  const glm::vec4& color)
	{
		s_Data->WhiteTexture->Bind();
		s_Data->TextureShader->UploadUniform("u_Texture", int(0));
		s_Data->TextureShader->UploadUniform("u_Color", color);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * /* TODO: rotation */ 
											 glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->UploadUniform("u_Transform", transform);
		s_Data->TextureVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->TextureVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position,
							  const glm::vec2& size,
							  const Texture2D& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position,
							  const glm::vec2& size,
							  const Texture2D& texture)
	{
		texture.Bind();
		s_Data->TextureShader->UploadUniform("u_Texture", int(0));
		s_Data->TextureShader->UploadUniform("u_Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * /* TODO: rotation */
											 glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->UploadUniform("u_Transform", transform);
		s_Data->TextureVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->TextureVertexArray);
	}

}