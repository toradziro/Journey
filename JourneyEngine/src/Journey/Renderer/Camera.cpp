#include "jnypch.h"
#include "Camera.h"
#include "Journey/Scene/Components.h"

namespace jny
{

void Camera::setViewportSize(u32 w, u32 h)
{
	m_aspectRatio = static_cast<f32>(w) / static_cast<f32>(h);
	setProjection();
}

void Camera::setProps(f32 zoom, f32 _near, f32 _far)
{
	m_zoom = zoom;
	m_near = _near;
	m_far = _far;
	setProjection();
}

void Camera::setZoom(f32 zoom)
{
	m_zoom = zoom;
	setProjection();
}

void Camera::setNear(f32 _near)
{
	m_near = _near;
	setProjection();
}

void Camera::setFar(f32 _far)
{
	m_near = _far;
	setProjection();
}

const glm::mat4& Camera::projection() const
{
	return m_projection;
}

void Camera::setProjection()
{
	f32 left = -m_aspectRatio;
	f32 right = m_aspectRatio;
	f32 bottom = -1.0f;
	f32 top = 1.0f;

	f32 hZoom = m_zoom * 0.5f;

	m_projection = glm::ortho(left * hZoom,
		right * hZoom,
		bottom * hZoom,
		top * hZoom,
		m_near, m_far);
}

}