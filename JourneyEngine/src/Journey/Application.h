#pragma once

#include "Core.h"

namespace jny
{

class JNY_API Application
{
public:
	Application();
	virtual ~Application();

	virtual void run();

private:
	bool m_running = true;
};

//-- Will be defined in client
Application* createApplication();

} //-- jny
