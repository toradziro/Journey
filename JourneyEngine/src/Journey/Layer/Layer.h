#pragma once

#include "jnypch.h"
#include "Journey/Core.h"
#include "Journey/Events/Event.h"

namespace jny
{

class JNY_API Layer
{
public:
	Layer(const std::string name) : m_layerName(name) {}
	
	virtual ~Layer() {}

	virtual void onEvent(Event& event) {}
	virtual void attach() {}
	virtual void detach() {}
	virtual void update() {}

private:
	std::string m_layerName;
};

} //-- jny