#include "jnypch.h"

#include "Application.h"
#include "Core.h"
#include "Log.h"
#include "Journey/Events/ApplicationEvent.h"

namespace jny
{

Application::Application()
{

}

Application::~Application()
{

}

void Application::run()
{
	Log::log(Log::LogLevel::trace, "Application::run {}", "start");

	WindowResizeEvent e(1000, 500);
	if (e.width() != 1000 || e.height() != 500)
	{
		Log::log(Log::LogLevel::error, "Event failed, wrong sizes: h({}) w({})", e.width(), e.height());
	}
	else
	{
		Log::log(Log::LogLevel::info, e.toString());
	}
	Log::log(Log::LogLevel::trace, "Application::run {}", "end");

	while (true)
	{
	}
}

} //-- jny