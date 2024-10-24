#pragma once

#include <glm/glm.hpp>

#include <Journey/Renderer/Texture.h>
#include <Journey/Renderer/Camera.h>
#include <Journey/Scene/Entity.h>

namespace jny
{

static inline constexpr auto C_PROP_NAME_HS = entt::hashed_string::value("property_name");
static inline constexpr auto C_ON_PROP_CHANGE_HS = entt::hashed_string::value("on_propety_change");
static inline constexpr auto C_FORMAT_STR_HS = entt::hashed_string::value("format_string");

void registerComponents();

struct TransformComponent
{
	static inline constexpr const char* C_COMPONENT_NAME = "Transform";

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;

	glm::mat4 transform() const
	{
		glm::mat4 ret { 1.0f };
		ret = glm::translate(ret, m_position);
		ret = glm::rotate(ret, m_rotation, glm::vec3{ 0.0f, 0.0f, 1.0f });
		ret = glm::scale(ret, m_scale);
		return ret;
	}

	glm::vec3	m_position = {};
	glm::vec3	m_scale = { 1.0f, 1.0f, 1.0f };
	f32			m_rotation = 0.0f;
};

struct SpriteComponent
{
	static inline constexpr const char* C_COMPONENT_NAME = "Sprite";

	SpriteComponent() = default;
	SpriteComponent(const SpriteComponent&) = default;

	glm::vec4		m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	Ref<Texture2D>	m_texture = nullptr;
};

struct EntityNameComponent
{
	static inline constexpr const char* C_COMPONENT_NAME = "EntityName";

	EntityNameComponent() = default;
	EntityNameComponent(const EntityNameComponent&) = default;

	std::string	m_name = {};
};

struct CameraComponent
{
	static inline constexpr const char* C_COMPONENT_NAME = "Camera";

	CameraComponent() = default;
	CameraComponent(const CameraComponent&) = default;

	Camera	m_camera = {};
	f32		m_zoom = 10.0f;
	bool	m_primer = false;
	bool	m_fixedAspectRatio = false;
};

struct NativeScriptComponent
{
	static inline constexpr const char* C_COMPONENT_NAME = "NativeScript";

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