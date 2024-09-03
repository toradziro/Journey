#pragma once

#include "Journey/Core/Core.h"
#include "Journey/Layer/Layer.h"

namespace jny
{

#ifdef JNY_PLATFORM_WINDOWS
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

class  LayerStack
{
public:
	using LayerStackPos = std::vector<Layer*>::iterator;
	using LayerReverseIt = std::vector<Layer*>::reverse_iterator;

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

	LayerReverseIt rbegin() { return m_layers.rbegin(); }
	LayerReverseIt rend() { return m_layers.rend(); }

private:
	std::vector<Layer*>		m_layers;
	uint32_t				m_position = 0;
};

#ifdef JNY_PLATFORM_WINDOWS
#pragma warning(pop)
#endif

} //-- jny
