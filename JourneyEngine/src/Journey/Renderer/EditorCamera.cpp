#include "jnypch.h"
#include "EditorCamera.h"
#include "Journey/Core/Application.h"
#include "Journey/Core/InputPoll.h"
#include "Journey/Events/MouseEvent.h"

#include <glfw/glfw3.h>

namespace jny
{

EditorCamera::EditorCamera(f32 aspectRatio
	, f32 fov
	, f32 nearPlane
	, f32 farPlane
	, u32 viewportW
	, u32 viewportH)
{
	m_camera.setProps(fov, nearPlane, farPlane);
	m_camera.setViewportSize(viewportW, viewportH);
	m_aspectRatio = aspectRatio;
	updateView();
}

void EditorCamera::update(f32 dt)
{
	m_lastDt = dt;

	auto& iPoll = Application::subsystems().st<InputPoll>();
	glm::vec3 offset = { 0.0f, 0.0f, 0.0f };
	const float moveStep = m_moveSpeed * dt;
	if (iPoll.mouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		if (!m_rightMouseButtonPressedLastFrame)
		{
			auto [mx, my] = iPoll.mousePos();
			m_mousePos = { mx, my };
			m_rightMouseButtonPressedLastFrame = true;
		}
		if (iPoll.keyPressed(GLFW_KEY_W))
		{
			offset.z += moveStep;
		}
		if (iPoll.keyPressed(GLFW_KEY_S))
		{
			offset.z -= moveStep;
		}
		if (iPoll.keyPressed(GLFW_KEY_A))
		{
			offset.x -= moveStep;
		}
		if (iPoll.keyPressed(GLFW_KEY_D))
		{
			offset.x += moveStep;
		}
		if (iPoll.keyPressed(GLFW_KEY_Q))
		{
			offset.y -= moveStep;
		}
		if (iPoll.keyPressed(GLFW_KEY_E))
		{
			offset.y += moveStep;
		}
		if (offset.x != 0.0f || offset.y != 0.0f || offset.z != 0.0f)
		{
			move(offset);
			updateView();
		}
	}
	else
	{
		m_rightMouseButtonPressedLastFrame = false;
	}
}

void EditorCamera::onEvent(Event& e)
{
	EventDispatcher ed(e);
	auto& iPoll = Application::subsystems().st<jny::InputPoll>();
	ed.dispatch<MouseMovedEvent>([&](MouseMovedEvent& e)
		{
			if (iPoll.mouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT) && m_rightMouseButtonPressedLastFrame)
			{
				if (e.getX() != m_mousePos.first || e.getY() != m_mousePos.second)
				{
					f32 deltaX = e.getX() - m_mousePos.first;
					f32 deltaY = e.getY() - m_mousePos.second;

					rotate(deltaX, deltaY);

					m_mousePos = { e.getX(), e.getY() };
				}
			}
			return true;
		});
}

const glm::mat4 EditorCamera::VPMatrix() const
{
	return m_camera.projection() * m_viewMatrix;
}

const glm::mat4 EditorCamera::viewMatrix() const
{
	return m_viewMatrix;
}

const glm::mat4 EditorCamera::projMatrix() const
{
	return m_camera.projection();
}

void EditorCamera::rotate(f32 deltaX, f32 deltaY)
{
	const float rotationSpeed = 0.05f;

	float angleX = deltaX * rotationSpeed * m_lastDt;
	float angleY = deltaY * rotationSpeed * m_lastDt;

	glm::vec3 right = glm::normalize(glm::rotate(m_orientation, glm::vec3(1.0f, 0.0f, 0.0f)));
	glm::vec3 up = glm::normalize(glm::rotate(m_orientation, glm::vec3(0.0f, 1.0f, 0.0f)));

	glm::quat qPitch = glm::angleAxis(-angleY, right);
	glm::quat qYaw = glm::angleAxis(-angleX, up);

	m_orientation = glm::normalize(qYaw * m_orientation * qPitch);

	updateView();
}

void EditorCamera::move(const glm::vec3& offset)
{
	glm::vec3 forward = glm::normalize(glm::rotate(m_orientation, glm::vec3(0.0f, 0.0f, -1.0f)));
	glm::vec3 right = glm::normalize(glm::rotate(m_orientation, glm::vec3(1.0f, 0.0f, 0.0f)));
	glm::vec3 up = glm::normalize(glm::rotate(m_orientation, glm::vec3(0.0f, 1.0f, 0.0f)));

	m_position += right * offset.x;
	m_position += up * offset.y;
	m_position += forward * offset.z;

	updateView();
}

void EditorCamera::onViewportResize(u32 viewportW, u32 viewportH)
{
	m_camera.setViewportSize(viewportW, viewportH);
}

void EditorCamera::updateView()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) * glm::mat4_cast(m_orientation);
	m_viewMatrix = glm::inverse(transform);
}

}