#pragma once

#include "Journey/Events/Event.h"

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
		m_title(title), m_width(width), m_height(height)
	{
	}
};

#pragma warning(push)
#pragma warning(disable: 4251)

class JNY_API Window
{
public:
	Window(WindowData data);
	~Window();

	void update();

	uint32_t height() const { return m_data.m_height; }
	uint32_t width() const { return m_data.m_width; }

	void setEventCallback(const WindowData::EventCallbackFn& callback) { m_data.m_eventCallback = callback; }
	
	void setVSync(bool enabled);
	bool isVSync() const { return m_data.m_vsyncEnabled; }

	void init();
	void shutdown();

private:
	WindowData		m_data;
	GLFWwindow*		m_window;
};

#pragma warning(pop)
#pragma warning(disable: 4251)

}
