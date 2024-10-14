#pragma once

#include "entt.hpp"
#include "Entity.h"
#include "Components.h"

namespace jny
{

class Scene : public ReferenceCounter
{
public:
	Scene();
	~Scene();

	void update(f32 dt);
	void onViewportResize(u32 width, u32 height);

	Entity createEntity()
	{
		Entity e = Entity(&m_registry);
		e.addComponent<TransformComponent>();
		e.addComponent<EntityNameComponent>();
		return e;
	}

private:
	//-- Scene entities and components storage
	entt::registry	m_registry;

	u32				m_viewportWidth = 0;
	u32				m_viewportHeight = 0;
};

} //-- jny