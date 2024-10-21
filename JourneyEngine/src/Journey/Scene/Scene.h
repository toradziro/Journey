#pragma once

#include "entt.hpp"
#include "Journey/Core/Reference.h"

namespace jny
{

class Entity;

class Scene : public ReferenceCounter
{
public:
	Scene();
	~Scene();

	void update(f32 dt);
	void onViewportResize(u32 width, u32 height);

	Entity createEntity();

	//-- This method can be used only in Scene Panel!
	entt::registry& registry()
	{
		return m_registry;
	}

private:
	//-- Scene entities and components storage
	entt::registry	m_registry;

	u32				m_viewportWidth = 0;
	u32				m_viewportHeight = 0;
};

} //-- jny