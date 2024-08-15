#pragma once

#include "jnypch.h"
#include "Journey/Core.h"
#include "Journey/Events/Event.h"

namespace jny
{

#pragma warning(push)
#pragma warning(disable: 4251)

class  Layer
{
public:
	Layer(const std::string name) : m_layerName(name) {}
	
	virtual ~Layer() {}

	virtual void onEvent(Event&) {}
	virtual void attach() {}
	virtual void detach() {}
	virtual void update(float /*dt*/) {}

	virtual void imGuiRender() {}

private:
	std::string m_layerName;
};

#pragma warning(pop)

} //-- jny