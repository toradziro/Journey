#pragma once

#include <glm/glm.hpp>

namespace jny
{

class OrthographicCamera
{
public:
	OrthographicCamera(float left, float right, float bottom, float top);

	inline void setPosition(const glm::vec3& position) { m_position = position; recalculateViewMatrix(); }
	//-- For 2D space we need only z rotation
	inline void setRotation(float rotation) { m_rotation = rotation; recalculateViewMatrix(); }

	const glm::vec3& position() { return m_position; }
	float rotation() const { return m_rotation; }

	const glm::mat4& projectionMatrix() { return m_projectionMatrix; }
	const glm::mat4& viewMatrix() { return m_viewMatrix; }
	const glm::mat4& viewProjectionMatrix() { return m_viewProjectionMatrix; }

private:
	void recalculateViewMatrix();

	glm::mat4	m_projectionMatrix = {};
	glm::mat4	m_viewMatrix = {};
	glm::mat4	m_viewProjectionMatrix = {};

	glm::vec3	m_position = {};
	float		m_rotation = 0.0f;
};

} //-- jny