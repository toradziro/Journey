#pragma once

#include "Journey/Core.h"
#include "Journey/Layer/LayerStack.h"
#include "SingletonInterface.h"

namespace jny
{

class ImGuiLayer;
class Window;

#ifdef JNY_PLATFORM_WINDOWS
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

class  Application
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

	static inline SingletonHolder& subsystems() { JNY_ASSERT(s_sHolder.get() != nullptr, "Don't know this systyem"); return *s_sHolder; }

private:
	LayerStack	m_layers;
	ImGuiLayer*	m_imGuiLayer = nullptr;
	bool		m_running = true;

private:
	static std::unique_ptr<SingletonHolder>	s_sHolder;
};

//-- Will be defined in client
Application* createApplication();

#ifdef JNY_PLATFORM_WINDOWS
#pragma warning(pop)
#endif

} //-- jny
