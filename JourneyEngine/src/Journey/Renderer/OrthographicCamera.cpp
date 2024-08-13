#include "jnypch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace jny
{

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	: m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_viewMatrix(1.0f)
{
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

void OrthographicCamera::recalculateViewMatrix()
{
	//-- Creates a matrix which is moving scene coordinates according to current position
	//-- glm::translate(glm::mat4(1.0f), m_position)

	//-- Creates rotation matrix (rotation in radians) and rotates around z axis
	//-- glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f))

	//-- Their combination is doing both operations - moves scene and rotates it

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));

	//-- Inversion is needed because we want to move scene in opposite direction of camera movement
	m_viewMatrix = glm::inverse(transform);
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

} //-- jny