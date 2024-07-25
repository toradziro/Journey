#include "jnypch.h"

#include "InputPool.h"
#include "Journey/Application.h" 

namespace jny
{

bool InputPool::keyPressed(int keycode)
{
	auto& win = Application::instance().st<Window>();
	int state = glfwGetKey(win.rawWindow(), keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool InputPool::mouseButtonPressed(int button)
{
	auto& win = Application::instance().st<Window>();
	int state = glfwGetMouseButton(win.rawWindow(), button);
	return state == GLFW_PRESS;
}

jny::MousePos InputPool::mousePos()
{
	auto& win = Application::instance().st<Window>();
	
	double xpos = 0.0;
	double ypos = 0.0;

	glfwGetCursorPos(win.rawWindow(), &xpos, &ypos);
	return { (float)xpos, float(ypos) };
}

} //-- jny