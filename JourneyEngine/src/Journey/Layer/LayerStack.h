#pragma once

#include "Journey/Core.h"
#include "Journey/Layer/Layer.h"
#include <EASTL/vector.h>

namespace jny
{

#pragma warning(push)
#pragma warning(disable: 4251)

class JNY_API LayerStack
{
public:
	using LayerStackPos = eastl::vector<Layer*>::iterator;

	LayerStack();
	~LayerStack();

	void push(Layer* layer);
	void pop(Layer* layer);

	void pushOverlay(Layer* layer);
	void popOverlay(Layer* layer);

	LayerStackPos begin() { return m_layers.begin(); }
	LayerStackPos end() { return m_layers.end(); }

private:
	eastl::vector<Layer*>	m_layers;
	uint32_t				m_position = 0;
};

#pragma warning(pop)

} //-- jny
