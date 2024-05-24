#include "vppch.h"
#include "OrthographicCameraController.h"
#include "Viper/Input/Input.h"
#include "Viper/Input/Keycodes.h"

namespace Viper {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Rotation(rotation), 
		  m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, 
				   -m_ZoomLevel, m_ZoomLevel)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep timestep)
	{
		if (m_Rotation)
		{
			if (Viper::Input::IsKeyPressed(VP_KEY_A))
			{
				m_CameraPosition.x -= glm::cos(m_CameraRotation) * m_CameraTranslationSpeed * timestep;
				m_CameraPosition.y -= glm::sin(m_CameraRotation) * m_CameraTranslationSpeed * timestep;
			}
			if (Viper::Input::IsKeyPressed(VP_KEY_D))
			{
				m_CameraPosition.x += glm::cos(m_CameraRotation) * m_CameraTranslationSpeed * timestep;
				m_CameraPosition.y += glm::sin(m_CameraRotation) * m_CameraTranslationSpeed * timestep;
			}
			if (Viper::Input::IsKeyPressed(VP_KEY_W))
			{
				m_CameraPosition.y += glm::cos(m_CameraRotation) * m_CameraTranslationSpeed * timestep;
				m_CameraPosition.x -= glm::sin(m_CameraRotation) * m_CameraTranslationSpeed * timestep;
			}
			if (Viper::Input::IsKeyPressed(VP_KEY_S))
			{
				m_CameraPosition.y -= glm::cos(m_CameraRotation) * m_CameraTranslationSpeed * timestep;
				m_CameraPosition.x += glm::sin(m_CameraRotation) * m_CameraTranslationSpeed * timestep;
			}
			if (Viper::Input::IsKeyPressed(VP_KEY_Q))
			{
				m_CameraRotation -= m_CameraRotationSpeed * timestep;
			}
			if (Viper::Input::IsKeyPressed(VP_KEY_E))
			{
				m_CameraRotation += m_CameraRotationSpeed * timestep;
			}
		}
		else
		{
			if (Viper::Input::IsKeyPressed(VP_KEY_A))
			{
				m_CameraPosition.x -= m_CameraTranslationSpeed * timestep;
			}
			if (Viper::Input::IsKeyPressed(VP_KEY_D))
			{
				m_CameraPosition.x += m_CameraTranslationSpeed * timestep;
			}
			if (Viper::Input::IsKeyPressed(VP_KEY_W))
			{
				m_CameraPosition.y += m_CameraTranslationSpeed * timestep;
			}
			if (Viper::Input::IsKeyPressed(VP_KEY_S))
			{
				m_CameraPosition.y -= m_CameraTranslationSpeed * timestep;
			}
		}
		if (Viper::Input::IsKeyPressed(VP_KEY_T)) 
		{ 
			m_CameraPosition = glm::vec3(0.0f); 
			m_CameraRotation = 0.0f; 
		}

		m_Camera.SetPosition(m_CameraPosition);
		if (m_Rotation) { m_Camera.SetRotation(m_CameraRotation); }
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(VP_BIND_EVENT_FUNC(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(VP_BIND_EVENT_FUNC(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_CameraTranslationSpeed = m_ZoomLevel;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel,
							   -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, 
								m_AspectRatio * m_ZoomLevel,
							   -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}