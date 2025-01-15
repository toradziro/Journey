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
static inline constexpr auto C_PROP_REMOVABLE = entt::hashed_string::value("componet_removable");
static inline constexpr auto C_ON_PROP_CHANGE_HS = entt::hashed_string::value("on_propety_change");
static inline constexpr auto C_FORMAT_STR_HS = entt::hashed_string::value("format_string");
static inline constexpr auto C_CASTOM_UI_DRAW = entt::hashed_string::value("castom_ui_draw");

void registerComponents();

struct TransformComponent
{
	static inline constexpr const char* C_COMPONENT_NAME = "Transform";

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;

	glm::mat4 transform() const
	{
		glm::mat4 transform { 1.0f };
		transform = glm::translate(transform, m_position);
		glm::mat4 rotationMatrix = glm::toMat4(glm::normalize(glm::quat(glm::radians(m_rotation))));
		transform = transform * rotationMatrix;
		transform = glm::scale(transform, m_scale);
		return transform;
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
		m_viewportW = std::max<u32>(1, w);
		m_viewportH = std::max<u32>(1, h);
		m_camera.setViewportSize(m_viewportW, m_viewportH);
		m_perspectiveCamera.setViewportSize(m_viewportW, m_viewportH);
	}

	Camera				m_camera = {};
	PerspectiveCamera	m_perspectiveCamera = {};
	f32					m_zoom = 10.0f; //-- works with ortho only
	f32					m_fov = 45.0f; //-- works with perspective only

	u32					m_viewportW = 1;
	u32					m_viewportH = 1;

	bool				m_primer = false;
	bool				m_fixedAspectRatio = false;
	bool				m_perspective = true;
};

struct MainHeroComponent
{
	static inline constexpr const char* C_COMPONENT_NAME = "Main Hero";

	MainHeroComponent() = default;
	MainHeroComponent(const MainHeroComponent&) = default;

	f32		m_movementSpeed = 5.0f;
};

} //-- jny