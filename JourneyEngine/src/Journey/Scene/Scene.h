#pragma once

#include "entt.hpp"

namespace jny
{

class Scene : public ReferenceCounter
{
public:
	Scene();
	~Scene();

	void update(f32 dt);

	entt::entity createEntity();
	entt::registry& registry() { return m_registry; }

private:
	//-- Scene entities and components storage
	entt::registry	m_registry;
};

} //-- jny