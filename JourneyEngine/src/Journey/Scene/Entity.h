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
	Entity(entt::registry* registry) : m_registry(registry)
	{
		m_entity = m_registry->create();
	}

public:
	Entity() = default;
	Entity(const Entity& other) : m_entity(other.m_entity), m_registry(other.m_registry) { }
	Entity operator=(const Entity& other)
	{
		m_entity = other.m_entity;
		m_registry = other.m_registry;
		return *this;
	}

	template<typename T, typename... Args>
	T& addComponent(Args&&... args)
	{
		JNY_ASSERT(!hasComponent<T>(), "Already here");

		return m_registry->emplace<T>(m_entity, std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	T& removeComponent(Args&&... args)
	{
		JNY_ASSERT(hasComponent<T>(), "No here yet");

		return m_registry->remove<T>(m_entity);
	}

	template<typename T>
	T& component()
	{
		JNY_ASSERT(hasComponent<T>(), "No here yet");

		return m_registry->get<T>(m_entity);
	}

	template<typename T>
	bool hasComponent()
	{
		return m_registry->all_of<T>(m_entity);
	}

	operator bool() const
	{
		return m_registry != nullptr && m_entity != entt::null;
	}

private:
	friend class Scene;

	entt::entity	m_entity = entt::null;
	entt::registry*	m_registry = nullptr;
};

class IScript
{
public:
	virtual ~IScript() = default;
	virtual void attach() = 0;
	virtual void detach() = 0;
	virtual void update() = 0;
};

template<typename Derived>
class Script : public IScript
{
public:
	Script(Entity& entity) : m_entity(entity) {}
	virtual ~Script() = default;

	template<typename T>
	T& component()
	{
		return m_entity.component<T>();
	}

	virtual void attach()
	{
		static_cast<Derived*>(this)->attach();
	}

	virtual void detach()
	{
		static_cast<Derived*>(this)->detach();
	}

	virtual void update()
	{
		static_cast<Derived*>(this)->update();
	}

private:
	Entity& m_entity;
};

}