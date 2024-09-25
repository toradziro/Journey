#pragma once

#include "Journey/Renderer/Buffer.h"

namespace jny
{

class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(uint32_t size);
	OpenGLVertexBuffer(float* vertices, uint32_t count);
	virtual ~OpenGLVertexBuffer();

	virtual void bind() const override;
	virtual void unbind() const override;

	virtual void setData(const void* data, uint32_t size) override;

	virtual void setLayout(const BufferLayout& layout) override;
	virtual const BufferLayout& layout() const override { return m_layout; }

private:
	BufferLayout	m_layout;
	uint32_t		m_rendererId = 0;
};

class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
	virtual ~OpenGLIndexBuffer();

	virtual void bind() const override;
	virtual void unbind() const override;

	virtual uint32_t count() const override { return m_indicesCount; }

private:
	uint32_t m_rendererId = 0;
	uint32_t m_indicesCount = 0;
};

}