#include "jnypch.h"
#include "Journey/OrthographicCameraController.h"
#include "Journey/Application.h"
#include "Journey/InputPoll.h"

#include "GLFW/glfw3.h"

namespace jny
{

OrthographicCameraController::OrthographicCameraController(float aspectRatio, float zoomLevel, bool rotation)
	: m_camera(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel),
	m_aspectRatio(aspectRatio),
	m_zoomLevel(zoomLevel)
{

}

void OrthographicCameraController::update(float dt)
{
	float cameraSpeedWithDeltaTime = m_cameraMoveSpeed * dt;

	auto& inputPollSystem = Application::subsystems().st<jny::InputPoll>();
	if (inputPollSystem.keyPressed(GLFW_KEY_D))
	{
		m_cameraPos.x += cameraSpeedWithDeltaTime;
	}
	else if (inputPollSystem.keyPressed(GLFW_KEY_A))
	{
		m_cameraPos.x -= cameraSpeedWithDeltaTime;
	}

	if (inputPollSystem.keyPressed(GLFW_KEY_W))
	{
		m_cameraPos.y += cameraSpeedWithDeltaTime;
	}
	else if (inputPollSystem.keyPressed(GLFW_KEY_S))
	{
		m_cameraPos.y -= cameraSpeedWithDeltaTime;
	}
	m_camera.setPosition(m_cameraPos);

	if (m_rotatable)
	{
		float cameraRotationWithDeltaTime = m_cameraRotationSpeed * dt;

		if (inputPollSystem.keyPressed(GLFW_KEY_Q))
		{
			m_cameraRotation += cameraRotationWithDeltaTime;
		}
		else if (inputPollSystem.keyPressed(GLFW_KEY_E))
		{
			m_cameraRotation -= cameraRotationWithDeltaTime;
		}
		m_camera.setRotation(m_cameraRotation);
	}
}

void OrthographicCameraController::onEvent(Event& event)
{
	jny::EventDispatcher dispatcher(event);
	dispatcher.dispatch<WindowResizeEvent>([this](WindowResizeEvent& e)
		{
			return windowResized(e);
		});

	dispatcher.dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& e)
		{
			return mouseScrolled(e);
		});
}

bool OrthographicCameraController::mouseScrolled(MouseScrolledEvent& e)
{
	m_zoomLevel -= e.offsetY() * 0.2f;
	m_zoomLevel = std::max(m_zoomLevel, 0.1f);
	m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);

	return false;
}

bool OrthographicCameraController::windowResized(WindowResizeEvent& e)
{
	m_aspectRatio = static_cast<float>(e.width()) / static_cast<float>(e.height());
	m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);

	return false;
}

} //-- jny