#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Viper {

	class Camera
	{
	public:
		Camera(float aspectRatio, float fov = glm::radians(80.0f), 
			   float nearPlane = 0.0f, float farPlane = 1000.0f);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const glm::quat& GetRotation() const { return m_Rotation; }
		void SetRotation(glm::quat rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; RecalculateProjectionMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
		void RecalculateProjectionMatrix();
	private:
		float m_AspectRatio;
		float m_FOV;
		float m_NearPlane;
		float m_FarPlane;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::quat m_Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	};

}
