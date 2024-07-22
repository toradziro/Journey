#pragma once

#include "Journey/Core.h"
#include "Journey/Window.h"
#include "Journey/Layer/LayerStack.h"

namespace jny
{

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

	void pushLayer(Layer* layer);
	void popLayer(Layer* layer);

	void pushOverlay(Layer* layer);
	void popOverlay(Layer* layer);

	inline Window& window() { return *m_window; }

	inline static Application& instance() { return *s_instance; }

private:
	LayerStack				m_layers;
	std::unique_ptr<Window>	m_window;
	bool					m_running = true;

private:
	static Application*		s_instance;
};

//-- Will be defined in client
Application* createApplication();

#pragma warning(pop)

} //-- jny
