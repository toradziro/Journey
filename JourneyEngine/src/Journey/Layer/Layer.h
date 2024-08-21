#pragma once

#include "jnypch.h"
#include "Journey/Core.h"
#include "Journey/Events/Event.h"

namespace jny
{

#ifdef JNY_PLATFORM_WINDOWS
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

class  Layer
{
public:
	Layer(const std::string name) : m_layerName(name) {}
	
	virtual ~Layer() = default;

	virtual void onEvent(Event&) {}
	virtual void attach() {}
	virtual void detach() {}
	virtual void update(float /*dt*/) {}

	virtual void imGuiRender() {}

private:
	std::string m_layerName;
};

#ifdef JNY_PLATFORM_WINDOWS
#pragma warning(pop)
#endif

} //-- jny