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
};

} //-- jny