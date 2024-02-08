#include "vppch.h"
#include "CameraController.h"
#include "Viper/Input/Input.h"
#include "Viper/Input/Keycodes.h"

namespace Viper {

	CameraController::CameraController(float aspectRatio)
		: m_Camera(aspectRatio)
	{
		m_CameraPosition = m_Camera.GetPosition();
		m_CameraRotation = m_Camera.GetRotation();
		m_LastXPosition = Input::GetMouseX();
		m_LastYPosition = Input::GetMouseY();
	}

	void CameraController::OnUpdate(Timestep timestep)
	{
		//VP_TRACE("Camera position: {0}, {1}, {2}", m_CameraPosition.x, m_CameraPosition.y, m_CameraPosition.z);
		//VP_TRACE("Camera rotation: {0}, {1}, {2}, {3}", m_CameraRotation.w, m_CameraRotation.x, m_CameraRotation.y, m_CameraRotation.z);

		// Position
		if (Viper::Input::IsKeyPressed(VP_KEY_A))
		{
			static glm::quat localLeft = glm::quat(0.0f, -1.0f, 0.0f, 0.0f);
			glm::quat worldLeft = m_CameraRotation * localLeft * glm::conjugate(m_CameraRotation);
			m_CameraPosition += glm::vec3(worldLeft.x, worldLeft.y, worldLeft.z) * (m_CameraTranslationSpeed * timestep);
		}
		if (Viper::Input::IsKeyPressed(VP_KEY_D))
		{
			static glm::quat localRight = glm::quat(0.0f, 1.0f, 0.0f, 0.0f);
			glm::quat worldRight = m_CameraRotation * localRight * glm::conjugate(m_CameraRotation);
			m_CameraPosition += glm::vec3(worldRight.x, worldRight.y, worldRight.z) * (m_CameraTranslationSpeed * timestep);
		}
		if (Viper::Input::IsKeyPressed(VP_KEY_W))
		{
			static glm::quat localForward = glm::quat(0.0f, 0.0f, 0.0f, -1.0f);
			glm::quat worldForward = m_CameraRotation * localForward * glm::conjugate(m_CameraRotation);
			m_CameraPosition += glm::vec3(worldForward.x, worldForward.y, worldForward.z) * (m_CameraTranslationSpeed * timestep);
		}
		if (Viper::Input::IsKeyPressed(VP_KEY_S))
		{
			static glm::quat localBack = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
			glm::quat worldBack = m_CameraRotation * localBack * glm::conjugate(m_CameraRotation);
			m_CameraPosition += glm::vec3(worldBack.x, worldBack.y, worldBack.z) * (m_CameraTranslationSpeed * timestep);
		}

		// Rotation
		float xPos = Input::GetMouseX();
		float yPos = Input::GetMouseY();
		if (xPos > m_LastXPosition) // rotate right
		{
			float angleRadians = -1.0f * m_CameraTranslationSpeed * (xPos - m_LastXPosition) * timestep;
			angleRadians = std::min(angleRadians, m_CameraTranslationSpeedMax);
			static const glm::vec3 axis = glm::vec3(0, 1, 0);
			glm::quat rotationQuaternion = glm::angleAxis(angleRadians, axis);
			m_CameraRotation *= rotationQuaternion;
		}
		else if (xPos < m_LastXPosition) // rotate left
		{
			float angleRadians = 1.0f * m_CameraTranslationSpeed * (m_LastXPosition - xPos) * timestep;
			angleRadians = std::min(angleRadians, m_CameraTranslationSpeedMax);
			static const glm::vec3 axis = glm::vec3(0, 1, 0);
			glm::quat rotationQuaternion = glm::angleAxis(angleRadians, axis);
			m_CameraRotation *= rotationQuaternion;
		}
		if (yPos > m_LastYPosition) // rotate down
		{
			float angleRadians = -1.0f * m_CameraTranslationSpeed * (yPos - m_LastYPosition) * timestep;
			angleRadians = std::min(angleRadians, m_CameraTranslationSpeedMax);
			static const glm::vec3 axis = glm::vec3(1, 0, 0);
			glm::quat rotationQuaternion = glm::angleAxis(angleRadians, axis);
			m_CameraRotation *= rotationQuaternion;
		}
		else if (yPos < m_LastXPosition) // rotate up
		{
			float angleRadians = 1.0f * m_CameraTranslationSpeed * (m_LastYPosition - yPos) * timestep;
			angleRadians = std::min(angleRadians, m_CameraTranslationSpeedMax);
			static const glm::vec3 axis = glm::vec3(1, 0, 0);
			glm::quat rotationQuaternion = glm::angleAxis(angleRadians, axis);
			m_CameraRotation *= rotationQuaternion;
		}
		m_LastXPosition = xPos;
		m_LastYPosition = yPos;

		// Reset
		if (Input::IsKeyPressed(VP_KEY_T)) 
		{
			m_CameraPosition = glm::vec3(0.0, 0.0, -1.0);
			m_CameraRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		}

		// TODO: bug with the camera rotating about the rotation axis
		// TODO: both will recaluclate, need to wait to recalculate
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(VP_BIND_EVENT_FUNC(CameraController::OnWindowResized));
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_Camera.SetAspectRatio((float)e.GetWidth() / (float)e.GetHeight());
		return false;
	}

}