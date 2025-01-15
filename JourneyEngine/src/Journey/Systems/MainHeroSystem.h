#pragma once

#include "SystemInterface.h"

namespace jny
{

class MainHeroSystem : public System
{
public:
	MainHeroSystem(entt::registry& registry) : System(registry) {}

	virtual void attach() {}
	virtual void detach() {}
	virtual void update(f32 dt);
};

}