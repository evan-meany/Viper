#pragma once

#include <Viper.h>

class ExampleLayer : public Viper::Layer
{
public:
	ExampleLayer(float aspectRatio);
	virtual void OnImGuiRender() override;
	virtual void OnUpdate(Viper::Timestep timestep) override;
	virtual void OnEvent(Viper::Event& evnt) override;
	bool OnKeyPressedEvent(Viper::KeyPressedEvent& evnt);
private:
	Viper::Shared<Viper::VertexArray> m_TriangleVertexArray;
	Viper::Shared<Viper::Shader> m_TriangleShader;

	Viper::Shared<Viper::VertexArray> m_SquareVertexArray;
	Viper::Shared<Viper::Shader> m_SquareShader;

	Viper::Shared<Viper::Shader> m_TextureShader;

	Viper::Shared<Viper::Texture2D> m_Texture;
	Viper::Shared<Viper::Texture2D> m_LogoTexture;

	bool m_CameraOn = true;
	//Viper::CameraController m_CameraController;
	Viper::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquarePosition = glm::vec3(0.5f);
	float m_SquarePositionSpeed = 1.0f;

	glm::vec4 m_Color = { 0.8, 0.3f, 0.1f, 1.0f };
};