#include "jnypch.h"

#include "Window.h"
#include "Journey/Log/Log.h"
#include "Journey/Events/ApplicationEvent.h"
#include "Journey/Events/KeyEvent.h"
#include "Journey/Events/MouseEvent.h"
#include "Journey/Renderer/OpenGL/OpenGlContext.h"

namespace
{

static jny::WindowData* getDataPtrFromWindow(GLFWwindow* window)
{
	JNY_ASSERT(window != nullptr)
	return (jny::WindowData*)glfwGetWindowUserPointer(window);
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
	Log::info("Creating window: width '{}' height '{}' title '{}'",
		m_data.m_width, m_data.m_height, m_data.m_title);

	if (!s_glfwInited)
	{
		int success = glfwInit();

		JNY_ASSERT(success == GLFW_TRUE)
		
		s_glfwInited = true;
		glfwSetErrorCallback([](int error, const char* description)
			{
				Log::error("GLFW error '{}' '{}'", error, description);
			});
	}

	m_window = glfwCreateWindow(static_cast<int>(m_data.m_width), static_cast<int>(m_data.m_height),
		m_data.m_title.c_str(), nullptr, nullptr);

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

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
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

	glfwSetCharCallback(m_window, [](GLFWwindow* window, uint32_t charcode)
		{
			WindowData* data = getDataPtrFromWindow(window);
			KeyTypedEvent event(charcode);
			data->m_eventCallback(event);
		});
	
	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
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