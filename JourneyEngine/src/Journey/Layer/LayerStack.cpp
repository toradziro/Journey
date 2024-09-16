#include "jnypch.h"
#include "LayerStack.h"

namespace jny
{

LayerStack::LayerStack()
{
}

LayerStack::~LayerStack()
{
	std::ranges::for_each(m_layers, [](auto& layer)
		{
			layer->detach();
			delete layer;
		});
}

void LayerStack::popLayer(Layer* layer)
{
	auto it = std::find(m_layers.begin(), m_layers.end(), layer);
	if (it != m_layers.end())
	{
		m_layers.erase(it);
		--m_position;
	}
	delete layer;
}

void LayerStack::popOverlay(Layer* layer)
{
	auto it = std::find(m_layers.begin(), m_layers.end(), layer);
	if (it != m_layers.end())
	{
		m_layers.erase(it);
	}
	delete layer;
}

} //-- jny