#include "jnypch.h"

#include "Journey/Application.h"
#include "Journey/Core.h"
#include "Journey/Log/Log.h"
#include "Journey/Events/ApplicationEvent.h"

#include <GLAD/glad.h>

namespace jny
{

Application::Application()
{
	m_window = std::unique_ptr<Window>(Window::create(WindowData("Journey", 1200, 800)));

	m_window->setEventCallback([this](Event& _event)
		{
			onEvent(_event);
		});
}

Application::~Application()
{

}

void Application::run()
{
	while (m_running)
	{
		glClearColor(0.2f, 0.5f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_window->update();
	}
}

void Application::onEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& event)
		{
			return windowCloseEvent();
		});

	for (auto& layer : m_layers)
	{
		layer->onEvent(event);
		if (event.handeled())
		{
			break;
		}
	}
}

bool Application::windowCloseEvent()
{
	m_running = false;
	return true;
}

void Application::pushLayer(Layer* layer)
{
	m_layers.push(layer);
}

void Application::popLayer(Layer* layer)
{
	m_layers.pop(layer);
}

void Application::pushOverlay(Layer* layer)
{
	m_layers.pushOverlay(layer);
}

void Application::popOverlay(Layer* layer)
{
	m_layers.popOverlay(layer);
}

} //-- jny