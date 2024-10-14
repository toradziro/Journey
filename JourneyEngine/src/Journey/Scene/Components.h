#pragma once

#include "jnypch.h"
#include <glm/glm.hpp>

#include <Journey/Renderer/Texture.h>
#include <Journey/Renderer/Camera.h>

namespace jny
{

struct TransformComponent
{
	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;

	glm::mat4	m_transform = { 1.0f };
};

struct SpriteComponent
{
	SpriteComponent() = default;
	SpriteComponent(const SpriteComponent&) = default;

	glm::vec4		m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec3		m_position = { 1.0f, 1.0f, 0.0f };
	glm::vec3		m_size = { 1.0f, 1.0f, 0.0f };
	Ref<Texture2D>	m_texture = nullptr;
	f32				m_rotation = { 0.0f };
	f32				m_rotationDegrees = { 0.0f };
};

struct EntityNameComponent
{
	EntityNameComponent() = default;
	EntityNameComponent(const EntityNameComponent&) = default;

	std::string	m_name = {};
};

struct CameraComponent
{
	CameraComponent() = default;
	CameraComponent(const CameraComponent&) = default;

	Camera	m_camera = {};
	bool	m_primer = false;
	bool	m_fixedAspectRatio = false;
};

} //-- jny