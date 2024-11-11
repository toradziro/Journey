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

/*
example:
	class CameraController : public Script
	{
	public:
		CameraController(Entity entity) : Script(entity) {}

		void attach() override {}

		void update(f32 dt) override
		{
			auto& tc = component<TransformComponent>();
			float cameraSpeedWithDeltaTime = m_cameraMoveSpeed * dt;
			auto& cameraPos = tc.m_position;

			auto& inputPoll = Application::subsystems().st<jny::InputPoll>();
			if (inputPoll.mouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
			{
				if (inputPoll.keyPressed(GLFW_KEY_D))
				{
					cameraPos.x += cameraSpeedWithDeltaTime;
				}
				else if (inputPoll.keyPressed(GLFW_KEY_A))
				{
					cameraPos.x -= cameraSpeedWithDeltaTime;
				}

				if (inputPoll.keyPressed(GLFW_KEY_W))
				{
					cameraPos.y += cameraSpeedWithDeltaTime;
				}
				else if (inputPoll.keyPressed(GLFW_KEY_S))
				{
					cameraPos.y -= cameraSpeedWithDeltaTime;
				}
			}
		}

		void detach() override {}

	private:
		f32	m_cameraMoveSpeed = 5.0f;
	};

	m_cameraE.addComponent<NativeScriptComponent>().bind<CameraController>(m_cameraE);
*/

}