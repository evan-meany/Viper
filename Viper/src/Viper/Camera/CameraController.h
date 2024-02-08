#pragma once
#include "Camera.h"

#include "Viper/Core/Timestep.h"
#include "Viper/Events/ApplicationEvent.h"
#include "Viper/Events/MouseEvent.h"

namespace Viper{

	class CameraController
	{
	public:
		CameraController(float aspectRatio);
		void OnUpdate(Timestep timestep);
		void OnEvent(Event& e);
		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }
	private:
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		Camera m_Camera;
		glm::vec3 m_CameraPosition;
		glm::quat m_CameraRotation;
		float m_CameraTranslationSpeed = 0.4f;
		float m_CameraTranslationSpeedMax = 1.5f;
		float m_CameraRotationSpeed = 2.0f;
		float m_LastXPosition;
		float m_LastYPosition;
	};

}
