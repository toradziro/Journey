#include "jnypch.h"
#include "OpenGLRendererAPI.h"

#include "Journey/Core/Profiling/TimeInstruments.h"

#include <glad/glad.h>

namespace jny
{

void OpenGLRendererAPI::init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_DEPTH_TEST);
}

void OpenGLRendererAPI::windowResized(uint32_t width, uint32_t height)
{
	glViewport(0, 0, width, height);
}

void OpenGLRendererAPI::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::setClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::drawIndexed(const Ref<VertexArray>& vertexArray)
{
	PROFILE_FUNC;

	vertexArray->bind();
	//-- Elements is indexes!
	glDrawElements(GL_TRIANGLES, vertexArray->indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
}

}