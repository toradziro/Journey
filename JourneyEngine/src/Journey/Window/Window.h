#pragma once

#include "Journey/Events/Event.h"
#include "Journey/Core/SingletonInterface.h"
#include "Journey/Renderer/GraphicContext.h"

#include <GLFW/glfw3.h>

namespace jny
{

struct WindowData
{
	using EventCallbackFn = std::function<void(Event&)>;

	std::string_view	m_title;
	u32			m_width;
	u32			m_height;
	float				m_dpiScale = 1.0f;
	EventCallbackFn		m_eventCallback;
	bool				m_vsyncEnabled = true;

	WindowData(std::string_view title, u32 width, u32 height) :
		m_title(title), m_width(width), m_height(height)
	{
	}
};

#ifdef JNY_PLATFORM_WINDOWS
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

class  Window : public ISingleton
{
	JNY_SINGLETON_TYPE(Window)
	Window(WindowData data);

public:
	~Window();

	void init();
	void shutdown();

	void update();

	u32 height() const { return m_data.m_height; }
	u32 width() const { return m_data.m_width; }

	void setEventCallback(const WindowData::EventCallbackFn& callback) { m_data.m_eventCallback = callback; }
	
	void setVSync(bool enabled);
	bool isVSync() const { return m_data.m_vsyncEnabled; }

	void hideCursor();
	void unhideCursor();

	float dpiScale() const { return m_data.m_dpiScale; }

	GLFWwindow* rawWindow() { return m_window; }

private:
	WindowData						m_data;
	std::unique_ptr<GraphicContext> m_context;
	GLFWwindow*						m_window;

	bool							m_fullScreen = true;
};

#ifdef JNY_PLATFORM_WINDOWS
#pragma warning(pop)
#pragma warning(disable: 4251)
#endif

}
