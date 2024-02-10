#include "vppch.h"
#include "CameraController.h"
#include "Viper/Input/Input.h"
#include "Viper/Input/Keycodes.h"
#include "Viper/Input/JoystickManager.h"

namespace Viper {

	const float deadzone = 0.1f;
	const float rotationMax = 1.5f;

	static void RotateCamera(float angleRadians, const glm::vec3& axis, glm::quat& currentRotation)
	{
		angleRadians = std::min(angleRadians, rotationMax);
		glm::quat rotationQuaternion = glm::angleAxis(angleRadians, axis);
		currentRotation *= rotationQuaternion;
	}

	CameraController::CameraController(float aspectRatio)
		: m_Camera(aspectRatio)
	{
		m_CameraPosition = m_Camera.GetPosition();
		m_CameraRotation = m_Camera.GetRotation();
		m_LastMouseXPosition = Input::GetMouseX();
		m_LastMouseYPosition = Input::GetMouseY();

		int joystickID = JoystickManager::GetPrimaryPlayerID();
		if (joystickID != -1) 
		{ 
			JoystickData data = Input::GetJoystickData(joystickID);
			m_LastJoystickRightXPosition = data.RightStickX;
			m_LastJoystickRightYPosition = data.RightStickY;
		}
	}

	void CameraController::OnUpdate(Timestep timestep)
	{
		//VP_TRACE("Camera position: {0}, {1}, {2}", m_CameraPosition.x, m_CameraPosition.y, m_CameraPosition.z);
		//VP_TRACE("Camera rotation: {0}, {1}, {2}, {3}", m_CameraRotation.w, m_CameraRotation.x, m_CameraRotation.y, m_CameraRotation.z);

		int joystickID = JoystickManager::GetPrimaryPlayerID();
		JoystickData data;
		if (joystickID != -1) { data = Input::GetJoystickData(joystickID); }
		VP_TRACE("data.LStickX: {0}", data.RightStickX);

		// Position
		if (Viper::Input::IsKeyPressed(VP_KEY_A) || (joystickID >= 0 && data.LeftStickX < -deadzone))
		{
			static glm::quat localLeft = glm::quat(0.0f, -1.0f, 0.0f, 0.0f);
			glm::quat worldLeft = m_CameraRotation * localLeft * glm::conjugate(m_CameraRotation);
			m_CameraPosition += glm::vec3(worldLeft.x, worldLeft.y, worldLeft.z) * (m_CameraTranslationSpeed * timestep);
		}
		if (Viper::Input::IsKeyPressed(VP_KEY_D) || (joystickID >= 0 && data.LeftStickX > deadzone))
		{
			static glm::quat localRight = glm::quat(0.0f, 1.0f, 0.0f, 0.0f);
			glm::quat worldRight = m_CameraRotation * localRight * glm::conjugate(m_CameraRotation);
			m_CameraPosition += glm::vec3(worldRight.x, worldRight.y, worldRight.z) * (m_CameraTranslationSpeed * timestep);
		}
		if (Viper::Input::IsKeyPressed(VP_KEY_W) || (joystickID >= 0 && data.LeftStickY < -deadzone))
		{
			static glm::quat localForward = glm::quat(0.0f, 0.0f, 0.0f, -1.0f);
			glm::quat worldForward = m_CameraRotation * localForward * glm::conjugate(m_CameraRotation);
			m_CameraPosition += glm::vec3(worldForward.x, worldForward.y, worldForward.z) * (m_CameraTranslationSpeed * timestep);
		}
		if (Viper::Input::IsKeyPressed(VP_KEY_S) || (joystickID >= 0 && data.LeftStickY > deadzone))
		{
			static glm::quat localBack = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
			glm::quat worldBack = m_CameraRotation * localBack * glm::conjugate(m_CameraRotation);
			m_CameraPosition += glm::vec3(worldBack.x, worldBack.y, worldBack.z) * (m_CameraTranslationSpeed * timestep);
		}

		// Rotation
		// TODO: finish controller rotation implementation. Maybe change this to 
		// have the controller info passed into the camera by the caller
		float mouseXPosition = Input::GetMouseX();
		float mouseYPosition = Input::GetMouseY();
		if (mouseXPosition > m_LastMouseXPosition || (joystickID >= 0 && data.RightStickX > m_LastJoystickRightXPosition)) // rotate right
		{
			float angleRadians = -1.0f * m_CameraTranslationSpeed * (mouseXPosition - m_LastMouseXPosition) * timestep;
			RotateCamera(angleRadians, glm::vec3(0, 1, 0), m_CameraRotation);
		}
		else if (mouseXPosition < m_LastMouseXPosition || (joystickID >= 0 && data.RightStickX < m_LastJoystickRightXPosition)) // rotate left
		{
			float angleRadians = 1.0f * m_CameraTranslationSpeed * (m_LastMouseXPosition - mouseXPosition) * timestep;
			angleRadians = std::min(angleRadians, m_CameraTranslationSpeedMax);
			static const glm::vec3 axis = glm::vec3(0, 1, 0);
			glm::quat rotationQuaternion = glm::angleAxis(angleRadians, axis);
			m_CameraRotation *= rotationQuaternion;
		}
		if (mouseYPosition > m_LastMouseYPosition || (joystickID >= 0 && data.RightStickY > m_LastJoystickRightYPosition)) // rotate down
		{
			float angleRadians = -1.0f * m_CameraTranslationSpeed * (mouseYPosition - m_LastMouseYPosition) * timestep;
			angleRadians = std::min(angleRadians, m_CameraTranslationSpeedMax);
			static const glm::vec3 axis = glm::vec3(1, 0, 0);
			glm::quat rotationQuaternion = glm::angleAxis(angleRadians, axis);
			m_CameraRotation *= rotationQuaternion;
		}
		else if (mouseYPosition < m_LastMouseYPosition || (joystickID >= 0 && data.RightStickY < m_LastJoystickRightYPosition)) // rotate up
		{
			float angleRadians = 1.0f * m_CameraTranslationSpeed * (m_LastMouseYPosition - mouseYPosition) * timestep;
			angleRadians = std::min(angleRadians, m_CameraTranslationSpeedMax);
			static const glm::vec3 axis = glm::vec3(1, 0, 0);
			glm::quat rotationQuaternion = glm::angleAxis(angleRadians, axis);
			m_CameraRotation *= rotationQuaternion;
		}
		m_LastMouseXPosition = mouseXPosition;
		m_LastMouseYPosition = mouseYPosition;
		m_LastJoystickRightXPosition = data.RightStickX;
		m_LastJoystickRightYPosition = data.RightStickY;

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