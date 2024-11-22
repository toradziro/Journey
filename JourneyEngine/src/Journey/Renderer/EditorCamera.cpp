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
	const float moveStep = m_moveSpeed * dt;
	if (iPoll.mouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		auto [mx, my] = iPoll.mousePos();
		m_mousePos = { mx, my };
		
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
	//updateView();
}

void EditorCamera::onViewportResize(u32 viewportW, u32 viewportH)
{
	m_camera.setViewportSize(viewportW, viewportH);
}

void EditorCamera::updateView()
{
	m_viewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

}