#include <jnypch.h>
#include "OpenGlContext.h"
#include "Journey/Core.h"

#include <GLFW/glfw3.h>
#include <GLAD/glad.h>

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
	Log::info("Vendor: {}", (char*)(glGetString(GL_VENDOR)));
	Log::info("Renderer: {}", (char*)(glGetString(GL_RENDERER)));
	Log::info("Version: {}", (char*)(glGetString(GL_VERSION)));
}

void OpenGlContext::swapBuffers()
{
	glfwSwapBuffers(m_window);
}

} //-- jny