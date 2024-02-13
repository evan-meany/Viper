#pragma once

#include "OrthographicCamera.h"

#include "Viper/Core/Timestep.h"
#include "Viper/Events/ApplicationEvent.h"
#include "Viper/Events/MouseEvent.h"

#include <glm/gtc/constants.hpp>

namespace Viper {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio = 16.0f / 9.0f, bool rotation = false);
		void OnUpdate(Timestep timestep);
		void OnEvent(Event& e);
		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = glm::vec3(0.0f);
		float m_CameraTranslationSpeed = m_ZoomLevel;

		bool m_Rotation;
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = glm::pi<float>();
	};

}