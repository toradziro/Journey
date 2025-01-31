#pragma once

#include <entt.hpp>
#include "Journey/Core/Reference.h"
#include "Journey/Renderer/EditorCamera.h"
#include "Journey/Systems/SystemInterface.h"
#include "SceneSerializer.h"

namespace jny
{

class Entity;
struct TransformComponent;
struct SpriteComponent;
struct EntityNameComponent;
struct CameraComponent;
struct MainHeroComponent;
struct RigidBodyComponent;
struct BoxColliderComponent;

class Scene
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

	void switchToGameMode();
	void switchToEditorMode();

private:
	template<typename T>
	void onComponentCreation(T& c) {}

	void onComponentCreation(CameraComponent& c);
	void copyToSnapshot();
	void restoreFromSnapshot();

private:
	friend class Entity;
	friend class SceneSerializer;
	//-- Scene entities and components storage
	entt::registry				m_registry;
	entt::registry				m_snapshot;

	std::string					m_sceneName = "Untitled";

	std::vector<u_ptr<System>>	m_gameSystems;

	u32							m_viewportWidth = 1;
	u32							m_viewportHeight = 1;
};

} //-- jny