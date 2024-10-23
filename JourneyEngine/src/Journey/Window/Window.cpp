#include "jnypch.h"

#include "Window.h"
#include "Journey/Log/Log.h"
#include "Journey/Events/ApplicationEvent.h"
#include "Journey/Events/KeyEvent.h"
#include "Journey/Events/MouseEvent.h"
#include "Journey/Renderer/OpenGL/OpenGlContext.h"

#include "Journey/Core/Profiling/TimeInstruments.h"

#ifdef JNY_PLATFORM_WINDOWS
	#define GLFW_EXPOSE_NATIVE_WIN32
	#include <GLFW/glfw3native.h>
	#include <Windows.h>
	#include <Dwmapi.h >
	#pragma comment(lib, "dwmapi.lib")
#endif

namespace
{

static jny::WindowData* getDataPtrFromWindow(GLFWwindow* window)
{
	JNY_ASSERT(window != nullptr, "Win nullptr")
	return (jny::WindowData*)glfwGetWindowUserPointer(window);
}

bool enableDarkTitleBar(HWND hwnd)
{
	const DWORD DWMWA_USE_IMMERSIVE_DARK_MODE = 20;

	BOOL useDarkMode = TRUE;
	HRESULT hr = DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode));
	return SUCCEEDED(hr);
}

} //-- unnamed

namespace jny
{

static bool s_glfwInited = false;

Window::Window(WindowData data)
	: m_data(std::move(data))
{
	init();
}

Window::~Window()
{
	shutdown();
}

void Window::update()
{
	PROFILE_FUNC;

	glfwPollEvents();
	m_context->swapBuffers();
}

void Window::setVSync(bool enabled)
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

void Window::init()
{
	PROFILE_FUNC;

	if (!s_glfwInited)
	{
		int success = glfwInit();

		JNY_ASSERT(success == GLFW_TRUE, "Hehe, how it can be?")
		
		s_glfwInited = true;
		glfwSetErrorCallback([](int error, const char* description)
			{
				Log::error("GLFW error '{}' '{}'", error, description);
			});

		float monScaleX = 0.0f;
		float monScaleY = 0.0f;
		glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &monScaleX, &monScaleY);

		m_data.m_dpiScale = std::max<float>(monScaleX, monScaleY);
		m_data.m_width = static_cast<u32>(static_cast<float>(m_data.m_width) * m_data.m_dpiScale);
		m_data.m_height = static_cast<u32>(static_cast<float>(m_data.m_height) * m_data.m_dpiScale);
	}

	Log::info("Creating window: width '{}' height '{}' title '{}'",
		m_data.m_width, m_data.m_height, m_data.m_title);

	m_window = glfwCreateWindow(static_cast<int>(m_data.m_width), static_cast<int>(m_data.m_height),
		m_data.m_title.data(), nullptr, nullptr);

#ifdef JNY_PLATFORM_WINDOWS
	HWND hwnd = glfwGetWin32Window(m_window);
	if (hwnd != nullptr)
	{
		enableDarkTitleBar(hwnd);
	}
#endif


	//-- Init for context will be called inside its constructor
	m_context = std::make_unique<OpenGlContext>(m_window);

	glfwSetWindowUserPointer(m_window, &m_data);
	setVSync(true);

	//-- Set callbacks
	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
		{
			WindowData* data = getDataPtrFromWindow(window);

			data->m_width = width;
			data->m_height = height;

			WindowResizeEvent event(width, height);
			data->m_eventCallback(event);
		});

	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
		{
			WindowData* data = getDataPtrFromWindow(window);
			WindowCloseEvent event;
			data->m_eventCallback(event);
		});

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
		{
			WindowData* data = getDataPtrFromWindow(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data->m_eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data->m_eventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data->m_eventCallback(event);
					break;
				}
				default:
					break;
			}
		});

	glfwSetCharCallback(m_window, [](GLFWwindow* window, u32 charcode)
		{
			WindowData* data = getDataPtrFromWindow(window);
			KeyTypedEvent event(charcode);
			data->m_eventCallback(event);
		});
	
	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int /*mods*/)
		{
			WindowData* data = getDataPtrFromWindow(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data->m_eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data->m_eventCallback(event);
					break;
				}
				default:
					break;
			}
		});

	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			WindowData* data = getDataPtrFromWindow(window);
			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			data->m_eventCallback(event);
		});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos)
		{
			WindowData* data = getDataPtrFromWindow(window);
			MouseMovedEvent event((float)xpos, (float)ypos);
			data->m_eventCallback(event);
		});
}

void Window::shutdown()
{
	glfwDestroyWindow(m_window);
}

} //-- jny