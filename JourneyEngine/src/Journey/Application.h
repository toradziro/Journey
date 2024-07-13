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
};

//-- Will be defined in client
Application* createApplication();

} //-- jny
