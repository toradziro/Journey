#pragma once

#include "Journey/Core.h"
#include "Journey/Window.h"
#include "Journey/Layer/LayerStack.h"

namespace jny
{

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

private:
	LayerStack				m_layers;
	std::unique_ptr<Window>	m_window;
	bool					m_running = true;
};

//-- Will be defined in client
Application* createApplication();

} //-- jny
