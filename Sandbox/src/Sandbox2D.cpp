#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(), m_Color(1.0f, 0.0f, 1.0f, 1.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_VertexArray = Viper::VertexArray::Create();
	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	auto squareVertexBuffer = Viper::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVertexBuffer->SetLayout({ { Viper::ShaderDataType::Float3, "a_Position" } });
	m_VertexArray->AddVertexBufffer(squareVertexBuffer);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	auto squareIndexBuffer = Viper::IndexBuffer::Create(squareIndices,
							 sizeof(squareIndices) / sizeof(uint32_t));

	m_VertexArray->SetIndexBufffer(squareIndexBuffer);
	m_Shader = Viper::Shader::Create("assets/shaders/Square.glsl");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Color Controller");
	ImGui::ColorPicker3("", glm::value_ptr(m_Color));
	ImGui::End();
}

void Sandbox2D::OnUpdate(Viper::Timestep timestep)
{
	m_CameraController.OnUpdate(timestep);

	Viper::RenderCommand::SetClearColor(glm::vec4(0.1f,0.1f,0.1f,1.0f));
	Viper::RenderCommand::Clear();

	Viper::Renderer::BeginScene(m_CameraController.GetCamera());

	m_Shader->Bind();
	m_Shader->UploadUniform("u_Color", m_Color);

	Viper::Renderer::Submit(m_Shader, m_VertexArray);

	Viper::Renderer::EndScene();
}

void Sandbox2D::OnEvent(Viper::Event& e)
{
	m_CameraController.OnEvent(e);
}
