#pragma once

#include "SystemInterface.h"
#include <box2d/box2d.h>

namespace jny
{

class PhysicSystem : public System
{
public:
	PhysicSystem(entt::registry& registry) : System(registry) {}

	virtual void attach();
	virtual void detach();
	virtual void update(f32 dt);

private:
	b2WorldId	m_worldId;
};

}