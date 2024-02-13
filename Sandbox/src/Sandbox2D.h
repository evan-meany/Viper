#pragma once

#include <Viper.h>

class Sandbox2D : public Viper::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;
	virtual void OnUpdate(Viper::Timestep timestep) override;
	virtual void OnEvent(Viper::Event& e) override;
private:
	Viper::OrthographicCameraController m_CameraController;

	// Below will be in 2D Renderer
	Viper::Shared<Viper::Shader> m_Shader;
	Viper::Shared<Viper::VertexArray> m_VertexArray;

	glm::vec4 m_Color;
};
