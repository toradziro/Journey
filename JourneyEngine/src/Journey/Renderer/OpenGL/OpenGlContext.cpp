#include <jnypch.h>
#include "OpenGlContext.h"
#include "Journey/Core/Core.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

namespace jny
{

OpenGlContext::OpenGlContext(GLFWwindow* window)
	: m_window(window)
{
	JNY_ASSERT(window, "Window is nullptr, make smth with that")

	init();
}

void OpenGlContext::init()
{
	glfwMakeContextCurrent(m_window);
	int gladInitStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	JNY_ASSERT(gladInitStatus, "Failed to init GLAD");

	Log::info("OpenGL Info:");
	Log::info("Vendor: {}", (const char*)(glGetString(GL_VENDOR)));
	Log::info("Renderer: {}", (const char*)(glGetString(GL_RENDERER)));
	Log::info("Version: {}", (const char*)(glGetString(GL_VERSION)));
}

void OpenGlContext::swapBuffers()
{
	glfwSwapBuffers(m_window);
}

} //-- jny