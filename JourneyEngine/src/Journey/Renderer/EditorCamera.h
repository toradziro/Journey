#pragma once

#include "Camera.h"
#include "Journey/Events/Event.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace jny
{

class EditorCamera
{
public:
	EditorCamera()
	{
		updateView();
	}
	EditorCamera(f32 aspectRatio
		, f32 fov = 45.0f
		, f32 nearPlane = 0.01f
		, f32 farPlane = 1000.0f
		, u32 viewportW = 1
		, u32 viewportH = 1
	);

	void onEvent(Event& e);
	void update(f32 dt);

	const glm::mat4 VPMatrix() const;
	const glm::mat4 viewMatrix() const;
	const glm::mat4 projMatrix() const;
	void rotate(f32 deltaX, f32 deltaY);

	void onViewportResize(u32 viewportW, u32 viewportH);

private:
	void updateView();
	void updateMousePos();

private:
	PerspectiveCamera	m_camera;
	glm::mat4			m_viewMatrix = glm::mat4(1.0f);
	std::pair<f32, f32>	m_mousePos = { 0.0f, 0.0f };

	glm::vec3			m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3			m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3			m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	f32					m_aspectRatio = 1.0f;
	f32					m_moveSpeed = 5.0f;
	f32					m_lastDt = 0.0f;
	f32					m_rotationSpeed = 0.07f;

	f32					m_yaw = -90.0f;
	f32					m_pitch = 0.0f;
};

}