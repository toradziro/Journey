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
	jny::Log::log(jny::Log::LogLevel::trace, "Application::run {}", "start");

	jny::WindowResizeEvent e(1000, 500);
	if (e.width() != 1000 || e.height() != 500)
	{
		jny::Log::log(jny::Log::LogLevel::error, "Event failed, wrong sizes: h({}) w({})", e.width(), e.height());
	}
	else
	{
		jny::Log::log(jny::Log::LogLevel::info, e.toString());
	}
	jny::Log::log(jny::Log::LogLevel::trace, "Application::run {}", "end");

	while (true)
	{
	}
}

} //-- jny