#pragma once

#include "Core.h"
#include "Journey/Window.h"

namespace jny
{

class JNY_API Application
{
public:
	Application();
	virtual ~Application();

	void run();

	void onEvent(Event& event);

private:
	std::unique_ptr<Window>	m_window;
	bool					m_running = true;
};

//-- Will be defined in client
Application* createApplication();

} //-- jny
