#pragma once

#include "jnypch.h"

#include <entt.hpp>
#include "Scene.h"
#include "Journey/Core/Core.h"

namespace jny
{

class Entity
{
protected:
	Entity(Scene* scene) : m_scene(scene)
	{
		m_entity = m_scene->registry().create();
	}

public:
	Entity() = default;
	Entity(entt::entity entity, Scene* scene) : m_entity(entity), m_scene(scene) {}
	Entity(const Entity& other) : m_entity(other.m_entity), m_scene(other.m_scene) { }
	Entity operator=(const Entity& other)
	{
		m_entity = other.m_entity;
		m_scene = other.m_scene;
		return *this;
	}

	template<typename T, typename... Args>
	T& addComponent(Args&&... args)
	{
		JNY_ASSERT(!hasComponent<T>(), "Already here");
		T& c = m_scene->registry().emplace<T>(m_entity, std::forward<Args>(args)...);
		m_scene->onComponentCreation(c);
		return c;
	}

	template<typename T, typename... Args>
	auto removeComponent(Args&&... args)
	{
		JNY_ASSERT(hasComponent<T>(), "No here yet");

		return m_scene->registry().remove<T>(m_entity);
	}

	template<typename T>
	T& component()
	{
		JNY_ASSERT(hasComponent<T>(), "No here yet");

		return m_scene->registry().get<T>(m_entity);
	}

	template<typename T>
	bool hasComponent()
	{
		return m_scene->registry().all_of<T>(m_entity);
	}

	entt::entity entityId() const { return m_entity; }

	operator bool() const
	{
		return m_scene != nullptr && m_entity != entt::null;
	}

	bool operator==(const Entity& other) const
	{
		return m_scene == other.m_scene && m_entity == other.m_entity;
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

private:
	friend class Scene;

	entt::entity	m_entity = entt::null;
	Scene*			m_scene = nullptr;
};

class Script
{
public:
	explicit Script(Entity entity) : m_entity(entity) {}
	virtual ~Script() = default;

	template<typename T>
	T& component()
	{
		return m_entity.component<T>();
	}

	virtual void attach() {}
	virtual void detach() {}
	virtual void update(f32 dt) {}

private:
	Entity m_entity;
};

}