#include "vppch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Viper {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		// This is a two part transform:
		// 
		// First a translation matrix is created. This is done by using gml::translate
		// gml::translate takes a base transformation matrix as input (a 4x4 identity in this case)
		// it then takes in a vec3 tranlation to apply to this matrix
		// 
		// Next a rotation matrix is calculated using glm::rotate. This takes an identity as before and applies
		// an angle to rotate. It also takes an axis to rotate about. By multiplying these two matrices we have
		// done the transformation

		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), m_Position) * 
			glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		// The inverse of the transform is taken because the world should be relative to the camera
		m_ViewMatrix = glm::inverse(transform);

		// Lastly the transformation (m_ViewMatrix) is multiplied by the ProjectionMatrix
		// this is an orthogonal projection matrix. 
		// From CHAT: An orthographic projection matrix transforms coordinates in an orthographic 
		// view volume to normalized device coordinates (NDC).
		// The matrix is defined by the dimensions of the view volume (left, right, bottom, top, near, far):
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; // Note: must be proj * view
	}

}