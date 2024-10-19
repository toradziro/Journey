#pragma once

#include "jnypch.h"

namespace jny
{

class IPanel
{
public:
	virtual void update(f32 dt) = 0;
	virtual void updateUI() = 0;
};

} //-- jny