#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace jny
{

class Camera
{
public:
	Camera() { setProjection(); }

	void setViewportSize(u32 w, u32 h);
	void setProps(f32 zoom, f32 _near, f32 _far);
	void setZoom(f32 zoom);
	void setNear(f32 _near);
	void setFar(f32 _far);
	const glm::mat4& projection() const;

private:
	void setProjection();

private:
	friend void registerComponents();

	glm::mat4	m_projection = { 1.0f };
	f32			m_zoom = 10.0f;
	f32			m_near = -1.0f;
	f32			m_far = 100.0f;
	f32			m_aspectRatio = 1.0f;
};

class PerspectiveCamera
{
public:
	PerspectiveCamera()
	{
		setProjection();
	}

	void setViewportSize(u32 w, u32 h);
	void setProps(f32 fov, f32 _near, f32 _far);
	void setFov(f32 fov);
	void setNear(f32 _near);
	void setFar(f32 _far);
	const glm::mat4& projection() const;

private:
	void setProjection();

private:
	friend void registerComponents();

	glm::mat4	m_projection = { 1.0f };
	f32			m_fov = 45.0f;
	f32			m_near = 0.01f;
	f32			m_far = 1000.0f;
	f32			m_aspectRatio = 1.0f;
};

}