#pragma once

#include "entt.hpp"

namespace jny
{

class Scene : public ReferenceCounter
{
public:
	Scene();
	~Scene();

private:
	//-- Scene entities and components storage
	entt::registry	m_registry;
};

} //-- jny