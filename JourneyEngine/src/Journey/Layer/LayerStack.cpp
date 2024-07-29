#include "jnypch.h"
#include "LayerStack.h"

namespace jny
{

LayerStack::LayerStack()
{
}

LayerStack::~LayerStack()
{
	for (auto layer : m_layers)
	{
		layer->detach();
		delete layer;
	}
}

void LayerStack::popLayer(Layer* layer)
{
	auto it = eastl::find(m_layers.begin(), m_layers.end(), layer);
	if (it != m_layers.end())
	{
		m_layers.erase(it);
		--m_position;
	}
	delete layer;
}

void LayerStack::popOverlay(Layer* layer)
{
	auto it = eastl::find(m_layers.begin(), m_layers.end(), layer);
	if (it != m_layers.end())
	{
		m_layers.erase(it);
	}
	delete layer;
}

} //-- jny