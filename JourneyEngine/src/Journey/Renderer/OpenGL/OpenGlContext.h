#pragma once

#include "Journey/Renderer/GraphicContext.h"

struct GLFWwindow;

namespace jny
{

class OpenGlContext : public GraphicContext
{
public:
	OpenGlContext(GLFWwindow* window);

	virtual void swapBuffers() override;

private:
	void init();

private:
	GLFWwindow* m_window;
};

}