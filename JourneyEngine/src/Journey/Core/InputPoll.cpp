#include "jnypch.h"

#include "InputPoll.h"
#include "Journey/Core/Application.h" 
#include "Journey/Window/Window.h"

namespace jny
{

bool InputPoll::keyPressed(int keycode)
{
	auto& win = Application::subsystems().st<Window>();

	int state = glfwGetKey(win.rawWindow(), keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool InputPoll::mouseButtonPressed(int button)
{
	auto& win = Application::subsystems().st<Window>();

	int state = glfwGetMouseButton(win.rawWindow(), button);
	return state == GLFW_PRESS;
}

MousePos InputPoll::mousePos()
{
	auto& win = Application::subsystems().st<Window>();

	double xpos = 0.0;
	double ypos = 0.0;

	glfwGetCursorPos(win.rawWindow(), &xpos, &ypos);
	return { static_cast<f32>(xpos), static_cast<f32>(ypos) };
}

} //-- jny