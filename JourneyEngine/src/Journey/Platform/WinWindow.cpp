#include "jnypch.h"
#include "WinWindow.h"
#include "Journey/Log.h"

namespace jny
{

static bool s_glfwInited = false;

Window* Window::create(WindowData data)
{
#ifdef JNY_PLATFORM_WINDOWS
	return new WinWindow(std::move(data));
#else
	return LinuxWindow(std::move(data));
#endif
}

WinWindow::WinWindow(WindowData data)
	: m_data(std::move(data))
{
	init();
}

WinWindow::~WinWindow()
{
	shutdown();
}

void WinWindow::update()
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}

void WinWindow::setVSync(bool enabled)
{
	m_data.m_vsyncEnabled = enabled;

	if (m_data.m_vsyncEnabled)
	{
		glfwSwapInterval(1);
	}
	else
	{
		glfwSwapInterval(0);
	}
}

void WinWindow::init()
{
	Log::log(Log::LogLevel::info, "Creating window: width '{}' height '{}' title '{}'",
		m_data.m_width, m_data.m_height, m_data.m_title);

	if (!s_glfwInited)
	{
		int success = glfwInit();

		JNY_ASSERT(success == GLFW_TRUE)
		
		s_glfwInited = true;
	}

	m_window = glfwCreateWindow(static_cast<int>(m_data.m_width), static_cast<int>(m_data.m_height),
		m_data.m_title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, &m_data);
	setVSync(true);
}

void WinWindow::shutdown()
{
	glfwDestroyWindow(m_window);
}

} //-- jny