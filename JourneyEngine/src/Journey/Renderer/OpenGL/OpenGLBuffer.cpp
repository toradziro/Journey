#include "jnypch.h"
#include "OpenGLBuffer.h"

#include <GLAD/glad.h>

namespace jny
{

//------------------------------------------------------------------------------------ Vertex buffer
OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t count)
{
	glCreateBuffers(1, &m_rendererId);
	bind();
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1, &m_rendererId);
}

void OpenGLVertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
}

void OpenGLVertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::setLayout(const BufferLayout& layout)
{
	m_layout = layout;
}

//------------------------------------------------------------------------------------ Index buffer
OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
	: m_indicesCount(count)
{
	glCreateBuffers(1, &m_rendererId);
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
	//-- Elements macro is indices in OpenGl
	glBufferData(GL_ARRAY_BUFFER, m_indicesCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	glDeleteBuffers(1, &m_rendererId);
}

void OpenGLIndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
}

void OpenGLIndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} //-- jny