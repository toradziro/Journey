#include "jnypch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace jny
{

void OpenGLRendererAPI::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::setClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::drawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
{
	vertexArray->bind();
	//-- Elements is indexes!
	glDrawElements(GL_TRIANGLES, vertexArray->indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
}

}