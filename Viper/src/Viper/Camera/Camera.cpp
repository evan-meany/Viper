#include "vppch.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Viper {

	static glm::mat4 ComputeProjectionMatrix(float aspectRatio, float fov, float nearPlane, float farPlane)
	{
		return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
	}

	static glm::mat4 ComputeViewMatrix(const glm::vec3& position, const glm::quat& rotation)
	{
		glm::quat inverseRotation = glm::conjugate(rotation);
		glm::mat4 rotationMatrix = glm::mat4_cast(inverseRotation);
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -position);
		return rotationMatrix * translationMatrix;
	}

	Camera::Camera(float aspectRatio, float fov, float nearPlane, float farPlane)
		: m_AspectRatio(aspectRatio), m_FOV(fov), m_NearPlane(nearPlane), m_FarPlane(farPlane)
	{
		m_ProjectionMatrix = ComputeProjectionMatrix(m_AspectRatio, m_FOV, m_NearPlane, m_FarPlane);
		m_ViewMatrix = ComputeViewMatrix(m_Position, m_Rotation);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::RecalculateViewMatrix()
	{
		m_ViewMatrix = ComputeViewMatrix(m_Position, m_Rotation);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::RecalculateProjectionMatrix()
	{
		m_ProjectionMatrix = ComputeProjectionMatrix(m_AspectRatio, m_FOV, m_NearPlane, m_FarPlane);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
