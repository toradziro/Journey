#pragma once

#include "Journey/Window.h"

namespace jny
{

class JNY_API WinWindow : public Window
{
public:
	WinWindow(WindowData data);
	~WinWindow();

	void update() override;

	uint32_t height() const override { return m_data.m_height; }
	uint32_t width() const override { return m_data.m_width; }

	void setEventCallback(const WindowData::EventCallbackFn& callback) override { m_data.m_eventCallback = callback; }
	
	void setVSync(bool enabled) override;
	bool isVSync() const override { return m_data.m_vsyncEnabled; }

	void init();
	void shutdown();

private:
	WindowData		m_data;
	GLFWwindow*		m_window;
};

}
