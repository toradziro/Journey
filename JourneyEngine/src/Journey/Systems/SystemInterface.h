#pragma once

#include <entt.hpp>

namespace jny
{

class System : public ReferenceCounter
{
public:
	System(entt::registry& registry) : m_registry(registry) {}

	virtual void attach() = 0;
	virtual void detach() = 0;
	virtual void update(f32 dt) = 0;

protected:
	entt::registry&		m_registry;
};

}