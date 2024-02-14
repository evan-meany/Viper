#include "vppch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

namespace Viper {

	struct Renderer2DData
	{
		Shared<VertexArray> VertexArray;
		Shared<Shader> Shader;
	};
	static Unique<Renderer2DData> s_Data;

	void Renderer2D::Init()
	{
		s_Data = std::make_unique<Renderer2DData>();

		s_Data->VertexArray = VertexArray::Create();
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		auto squareVertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVertexBuffer->SetLayout({ { ShaderDataType::Float3, "a_Position" } });
		s_Data->VertexArray->AddVertexBufffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		auto squareIndexBuffer = IndexBuffer::Create(squareIndices,
			sizeof(squareIndices) / sizeof(uint32_t));

		s_Data->VertexArray->SetIndexBufffer(squareIndexBuffer);
		s_Data->Shader = Shader::Create("assets/shaders/Square.glsl");
	}

	void Renderer2D::Shutdown()
	{
		s_Data.release();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->Shader->Bind();
		s_Data->Shader->UploadUniform("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data->Shader->UploadUniform("u_Transform", glm::mat4(1.0f));
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
		s_Data->Shader->Bind();
		s_Data->Shader->UploadUniform("u_Color", color);
		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}

}