#pragma once

#include "jnypch.h"
#include <glm/glm.hpp>

#include <Journey/Renderer/Texture.h>
#include <Journey/Renderer/Camera.h>
#include <Journey/Scene/Entity.h>

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

struct NativeScriptComponent
{
	template<typename T>
	void bind(Entity entity)
	{
		static_assert(std::is_base_of<Script, T>::value, "Fix lol.");
		m_createScript = [entity]()
			{
				return static_cast<Script*>(new T(entity));
			};

		m_destroyScript = [entity](NativeScriptComponent& nsc)
			{
				delete static_cast<T*>(nsc.m_script);
				nsc.m_script = nullptr;
			};
	}

	std::function<Script*()>					m_createScript;
	std::function<void(NativeScriptComponent&)>	m_destroyScript;

	Script*										m_script = nullptr;
};

} //-- jny