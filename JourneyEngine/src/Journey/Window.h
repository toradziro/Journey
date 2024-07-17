#pragma once

#include "jnypch.h"
#include "Events/Event.h"
#include <GLFW/glfw3.h>

namespace jny
{


struct WindowData
{
	using EventCallbackFn = std::function<void(Event&)>;

	std::string		m_title;
	uint32_t		m_width;
	uint32_t		m_height;
	EventCallbackFn	m_eventCallback;
	bool			m_vsyncEnabled = true;

	WindowData(std::string title, uint32_t width, uint32_t height) :
		m_title(title), m_width(width), m_height(height) { }
};


class Window
{
public:
	virtual ~Window() {}

	virtual void update() = 0;
	
	virtual uint32_t height() const = 0;
	virtual uint32_t width() const = 0;

	virtual void setEventCallback(const WindowData::EventCallbackFn& callback) = 0;
	virtual void setVSync(bool enabled) = 0;
	virtual bool isVSync() const = 0;

	static Window* create(WindowData = WindowData("Journey", 1200, 800));
};

}