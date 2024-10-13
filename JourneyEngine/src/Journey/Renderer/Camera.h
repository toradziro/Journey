#pragma once

#include <glm/glm.hpp>

namespace jny
{

class Camera
{
public:
	Camera() = default;
	Camera(glm::mat4 projection) : m_projection(projection) {}

	const glm::mat4& projection() const { return m_projection; }

private:
	glm::mat4	m_projection = { 1.0f };
};

}