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

void OpenGLRendererAPI::windowResized(u32 width, u32 height)
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

void OpenGLRendererAPI::clearRedIntTexture(u32 textureIndex, i32 value)
{
	glClearTexImage(textureIndex, 0, GL_RED_INTEGER, GL_INT, &value);
}

void OpenGLRendererAPI::drawIndexed(const s_ptr<VertexArray>& vertexArray, u32 count)
{
	PROFILE_FUNC;

	u32 indiciesCount = count == 0 ? vertexArray->indexBuffer()->count() : count;

	vertexArray->bind();
	//-- Elements is indexes!
	glDrawElements(GL_TRIANGLES, indiciesCount, GL_UNSIGNED_INT, nullptr);
}

}