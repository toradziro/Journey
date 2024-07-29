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

	template<typename T, typename ...Args>
	T* pushLayer(Args&&... args)
	{
		static_assert(std::is_base_of<Layer, T>::value, "To layer your layer must be inherited from Layer");

		T* layerInstance = new T(std::forward<Args>(args)...);
		layerInstance->attach();
		
		m_layers.emplace(begin() + m_position, static_cast<Layer*>(layerInstance));
		++m_position;
		
		return layerInstance;
	}

	template<typename T, typename ...Args>
	T* pushOverlay(Args&&... args)
	{
		T* layerInstance = new T(std::forward<Args>(args)...);
		layerInstance->attach();

		m_layers.emplace_back(static_cast<Layer*>(layerInstance));
		
		return layerInstance;
	}

	void popLayer(Layer* layer);
	void popOverlay(Layer* layer);

	LayerStackPos begin() { return m_layers.begin(); }
	LayerStackPos end() { return m_layers.end(); }

private:
	eastl::vector<Layer*>	m_layers;
	uint32_t				m_position = 0;
};

#pragma warning(pop)

} //-- jny
