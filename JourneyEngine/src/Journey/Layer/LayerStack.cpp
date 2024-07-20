#include "jnypch.h"
#include "LayerStack.h"

namespace jny
{

LayerStack::LayerStack()
{
	m_position = m_layers.begin();
}

LayerStack::~LayerStack()
{
	for (auto layer : m_layers)
	{
		delete layer;
	}
}

void LayerStack::push(Layer* layer)
{
	m_position = m_layers.emplace(m_position, layer);
}

void LayerStack::pop(Layer* layer)
{
	auto it = eastl::find(m_layers.begin(), m_layers.end(), layer);
	if (it != m_layers.end())
	{
		m_layers.erase(it);
		--m_position;
	}
}

void LayerStack::pushOverlay(Layer* layer)
{
	m_layers.emplace_back(layer);
}

void LayerStack::popOverlay(Layer* layer)
{
	auto it = eastl::find(m_layers.begin(), m_layers.end(), layer);
	if (it != m_layers.end())
	{
		m_layers.erase(it);
	}
}

} //-- jny