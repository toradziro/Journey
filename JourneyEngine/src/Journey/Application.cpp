#include "jnypch.h"

#include "Application.h"
#include "Core.h"
#include "Log.h"

namespace jny
{

Application::Application()
{
	m_window = std::unique_ptr<Window>(Window::create(WindowData("Journey", 1200, 800)));

	m_window->setEventCallback([this](Event& event)
		{
			onEvent(event);
		});
}

Application::~Application()
{

}

void Application::run()
{
	Log::log(Log::LogLevel::trace, "Application::run {}", "start");

	while (m_running)
	{
		glClearColor(0.2f, 0.5f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_window->update();
	}
}

void Application::onEvent(Event& event)
{
	Log::log(Log::LogLevel::info, event.toString());
}

} //-- jny