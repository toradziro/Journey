#pragma once

#include "Journey/Core.h"
#include "Journey/Layer/LayerStack.h"
#include "SingletonInterface.h"

namespace jny
{

class ImGuiLayer;
class Window;

#pragma warning(push)
#pragma warning(disable: 4251)

class JNY_API Application
{
public:
	Application();
	virtual ~Application();

	void run();

	void onEvent(Event& event);
	bool windowCloseEvent();


	template<typename T, typename ...Args>
	T* pushLayer(Args&&... args)
	{
		return m_layers.pushLayer<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename ...Args>
	T* pushOverlay(Args&&... args)
	{
		return m_layers.pushOverlay<T>(std::forward<Args>(args)...);
	}

	void popLayer(Layer* layer);
	void popOverlay(Layer* layer);

	static inline SingletonHolder& instance() { JNY_ASSERT(s_sHolder != nullptr); return *s_sHolder; }

private:
	LayerStack	m_layers;
	ImGuiLayer*	m_imGuiLayer;
	bool		m_running = true;

private:
	static std::unique_ptr<SingletonHolder>	s_sHolder;
};

//-- Will be defined in client
Application* createApplication();

#pragma warning(pop)

} //-- jny
