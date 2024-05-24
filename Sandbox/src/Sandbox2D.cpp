#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(), m_Color(1.0f, 0.0f, 1.0f, 1.0f)
{
	m_CameraController.SetRotation(true);
	m_Texture = Viper::Texture2D::Create("assets/textures/Clefairy.png");
}

void Sandbox2D::OnAttach()
{

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

	Viper::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Viper::Renderer2D::DrawQuad({0.0f, 0.0f, 0.1f}, { 0.5f, 0.5f }, *m_Texture);
	Viper::Renderer2D::DrawQuad({0.0f, 0.0f}, { 1.0f, 1.0f }, m_Color);
	Viper::Renderer2D::DrawQuad({1.0f, 1.0f}, { 1.0f, 1.0f }, m_Color);
	Viper::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(Viper::Event& e)
{
	m_CameraController.OnEvent(e);
}
