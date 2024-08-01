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
	int gradInitStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	JNY_ASSERT(status, "Failed to init GLAD")
}

void OpenGlContext::swapBuffers()
{
	glfwSwapBuffers(m_window);
}

}