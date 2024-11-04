#include "jnypch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace jny
{

namespace
{

GLenum toOpenGLType(buff_utils::ShaderDataType type)
{
	using namespace buff_utils;
	switch (type)
	{
		case ShaderDataType::Bool:		return GL_BOOL;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
			//-- 3x3 matrix
		case ShaderDataType::Mat3:		return GL_FLOAT;
			//-- 4x4 matrix
		case ShaderDataType::Mat4:		return GL_FLOAT;
		default:
			break;
	}

	JNY_ASSERT(false, "Invalid type");
	return 0;
}

} //-- unnamed

OpenGLVertexArray::OpenGLVertexArray()
{
	glCreateVertexArrays(1, &m_rendererId);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
	glDeleteVertexArrays(1, &m_rendererId);
}

void OpenGLVertexArray::bind() const
{
	glBindVertexArray(m_rendererId);
}

void OpenGLVertexArray::unbind() const
{
	glBindVertexArray(0);
}

void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
{
	JNY_ASSERT(!vertexBuffer->layout().elements().empty(), "Empty layout");

	glBindVertexArray(m_rendererId);
	vertexBuffer->bind();

	std::ranges::for_each(vertexBuffer->layout(), [&](const auto& element)
		{
			//-- Enable 0 layout in our shader
			glEnableVertexAttribArray(m_vertexBufferIndexOffset);
			//-- Attribute pointer applied to EVERY VERTEX, not to all data
			//-- which means that:
			//-- index - index of layout we are setting up
			//-- size - amount of elements in that layout
			//-- type - data type for every element in that layout
			//-- stride - size in bytes for all of elements in this layout (corresponding to vertex)
			//-- offset - how much bytes need to shift in vertexes to reach a start address of this layout from the start of the vertex
			glVertexAttribPointer(
				m_vertexBufferIndexOffset,
				element.m_count,
				toOpenGLType(element.m_type),
				element.m_normilized ? GL_TRUE : GL_FALSE,
				vertexBuffer->layout().stride(),
				reinterpret_cast<void*>(static_cast<uintptr_t>(element.m_offset))
			);

			++m_vertexBufferIndexOffset;
		});

	m_vertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& IndexBuffer)
{
	glBindVertexArray(m_rendererId);
	IndexBuffer->bind();

	m_indexBuffer = IndexBuffer;
}

} //-- jny