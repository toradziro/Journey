#include "jnypch.h"

#include "Application.h"
#include "Core.h"
#include "Log.h"
#include "Journey/Events/ApplicationEvent.h"
#include "Journey/Window.h"

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

	Window* win = Window::create();

	while (m_running)
	{
		glClearColor(0.2f, 0.5f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		win->update();
	}
}

} //-- jny