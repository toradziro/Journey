#pragma once

#include "Journey/Core/Reference.h"
#include "Journey/Renderer/OrthographicCamera.h"
#include "Journey/Events/ApplicationEvent.h"
#include "Journey/Events/MouseEvent.h"

namespace jny
{

class OrthographicCameraController : public ReferenceCounter
{
public:
	OrthographicCameraController(float aspectRatio, float zoomLevel = 1.0f, bool rotation = false);

	void update(float dt);
	void onEvent(Event& event);

	const OrthographicCamera& camera() const { return m_camera; }

private:
	bool mouseScrolled(MouseScrolledEvent& e);
	bool windowResized(WindowResizeEvent& e);

	OrthographicCamera	m_camera;

	glm::vec3			m_cameraPos = { 0.0f, 0.0f, 0.0f };
	float				m_cameraMoveSpeed = 1.8f;

	float				m_aspectRatio;
	float				m_zoomLevel = 1.0f;

	float				m_cameraRotationSpeed = 40.0f;
	float				m_cameraRotation = 0.0f;
	bool				m_rotatable = false;
};

} //-- jny