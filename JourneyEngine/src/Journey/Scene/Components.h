#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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
		glm::mat4 rotationMatrix = glm::toMat4(glm::normalize(glm::quat(glm::radians(m_rotation))));
		ret = ret * rotationMatrix;
		ret = glm::scale(ret, m_scale);
		return ret;
	}

	glm::vec3	m_position = {};
	glm::vec3	m_scale = { 1.0f, 1.0f, 1.0f };
	glm::vec3	m_rotation = { 0.0f, 0.0f, 0.0f };
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

	glm::mat4 projection() const
	{
		if (m_perspective)
		{
			return m_perspectiveCamera.projection();
		}
		else
		{
			return m_camera.projection();
		}
	}

	void onViewportResize(u32 w, u32 h)
	{
		//-- Need to avoid -(nan)
		u32 uw = std::max<u32>(1, w);
		u32 uh = std::max<u32>(1, h);
		m_camera.setViewportSize(uw, uh);
		m_perspectiveCamera.setViewportSize(uw, uh);
	}

	Camera				m_camera = {};
	PerspectiveCamera	m_perspectiveCamera = {};
	f32					m_zoom = 10.0f; //-- works with ortho only
	f32					m_fov = 45.0f; //-- works with perspective only
	bool				m_primer = false;
	bool				m_fixedAspectRatio = false;
	bool				m_perspective = false;
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