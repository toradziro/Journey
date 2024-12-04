#include "jnypch.h"
#include "EditorCamera.h"
#include "Journey/Core/Application.h"
#include "Journey/Core/InputPoll.h"
#include "Journey/Events/MouseEvent.h"
#include "Journey/Window/Window.h"

#include <glfw/glfw3.h>

namespace
{

constexpr glm::vec3 C_WORLD_UP = { 0.0f, 1.0f, 0.0f };

}

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
	const float moveStep = m_moveSpeed * dt;
	if (iPoll.mouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		if (iPoll.keyPressed(GLFW_KEY_W))
		{
			m_cameraPos += moveStep * m_cameraFront;
			updateView();
		}
		if (iPoll.keyPressed(GLFW_KEY_S))
		{
			m_cameraPos -= moveStep * m_cameraFront;
			updateView();
		}
		if (iPoll.keyPressed(GLFW_KEY_A))
		{
			m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * moveStep;
			updateView();
		}
		if (iPoll.keyPressed(GLFW_KEY_D))
		{
			m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * moveStep;
			updateView();
		}
		if (iPoll.keyPressed(GLFW_KEY_Q))
		{
			m_cameraPos -= moveStep * m_cameraUp;
			updateView();
		}
		if (iPoll.keyPressed(GLFW_KEY_E))
		{
			m_cameraPos += moveStep * m_cameraUp;
			updateView();
		}
	}
}

void EditorCamera::onEvent(Event& e)
{
	EventDispatcher ed(e);
	auto& iPoll = Application::subsystems().st<jny::InputPoll>();
	ed.dispatch<MouseMovedEvent>([&](MouseMovedEvent& e)
		{
			if (iPoll.mouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
			{
				if (!m_startProcessMovementEvents)
				{
					m_startProcessMovementEvents = true;
					m_mousePos.first = e.getX();
					m_mousePos.second = e.getY();
				}
				f32 deltaX = e.getX() - m_mousePos.first;
				f32 deltaY = e.getY() - m_mousePos.second;

				rotate(deltaX, deltaY);
				updateMousePos();
				return true;
			}
			return false;
		});

	ed.dispatch<MouseButtonReleasedEvent>([&](MouseButtonReleasedEvent& e)
		{
			if (e.buttonCode() == GLFW_MOUSE_BUTTON_RIGHT)
			{
				m_startProcessMovementEvents = false;
			}
			return false;
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
	m_yaw += deltaX * m_rotationSpeed;
	m_pitch -= deltaY * m_rotationSpeed;

	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}
	if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}

	m_cameraFront.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
	m_cameraFront.y = sin(glm::radians(m_pitch));
	m_cameraFront.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));

	m_cameraRight = glm::normalize(glm::cross(m_cameraFront, C_WORLD_UP));
	m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraFront));
	updateView();
}

void EditorCamera::onViewportResize(u32 viewportW, u32 viewportH)
{
	m_camera.setViewportSize(viewportW, viewportH);
}

void EditorCamera::updateView()
{
	m_viewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

void EditorCamera::updateMousePos()
{
	auto& iPoll = Application::subsystems().st<InputPoll>();
	MousePos mPos = iPoll.mousePos();
	m_mousePos = std::pair<f32, f32>(mPos.x, mPos.y);
}

}