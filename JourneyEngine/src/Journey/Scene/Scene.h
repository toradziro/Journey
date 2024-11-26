#pragma once

#include <entt.hpp>
#include "Journey/Core/Reference.h"
#include "Journey/Renderer/EditorCamera.h"
#include "SceneSerializer.h"

namespace jny
{

class Entity;
struct TransformComponent;
struct SpriteComponent;
struct EntityNameComponent;
struct CameraComponent;
struct NativeScriptComponent;

class Scene : public ReferenceCounter
{
public:
	Scene();
	~Scene();

	void update(f32 dt);
	void editorModeUpdate(f32 dt, const EditorCamera& camera);
	void onViewportResize(u32 width, u32 height);

	Entity createEntity();
	void removeEntity(Entity e);
	void removeEntity(entt::entity e);

	//-- This method can be used only in Scene Panel!
	entt::registry& registry()
	{
		return m_registry;
	}

	std::string name() const { return m_sceneName; }
	void setName(const std::string& name) { m_sceneName = name; }

	void serialize(const std::string& filename);
	void deserialize(const std::string& filename);

	Entity activeCameraEntity();

private:
	void onComponentCreation(TransformComponent& c) {}
	void onComponentCreation(SpriteComponent&c ) {}
	void onComponentCreation(EntityNameComponent& c) {}
	void onComponentCreation(CameraComponent& c);
	void onComponentCreation(NativeScriptComponent& c) {}

private:
	friend class Entity;
	friend class SceneSerializer;
	//-- Scene entities and components storage
	entt::registry	m_registry;

	std::string		m_sceneName = "Untitled";

	u32				m_viewportWidth = 1;
	u32				m_viewportHeight = 1;
};

} //-- jny