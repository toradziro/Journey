#include "jnypch.h"
#include "OpenGLBuffer.h"

#include <GLAD/glad.h>

namespace jny
{

//------------------------------------------------------------------------------------ Vertex buffer
OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
{
	glCreateBuffers(1, &m_rendererId);
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t count)
{
	glCreateBuffers(1, &m_rendererId);
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
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

void OpenGLVertexBuffer::setData(const void* data, uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
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