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
	glDepthFunc(GL_LESS);
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

void OpenGLRendererAPI::drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count)
{
	PROFILE_FUNC;

	uint32_t indiciesCount = count == 0 ? vertexArray->indexBuffer()->count() : count;

	vertexArray->bind();
	//-- Elements is indexes!
	glDrawElements(GL_TRIANGLES, indiciesCount, GL_UNSIGNED_INT, nullptr);
}

}