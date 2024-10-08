#pragma once

#include "Journey/Renderer/Buffer.h"

namespace jny
{

class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(u32 size);
	OpenGLVertexBuffer(float* vertices, u32 count);
	virtual ~OpenGLVertexBuffer();

	virtual void bind() const override;
	virtual void unbind() const override;

	virtual void setData(const void* data, u32 size) override;

	virtual void setLayout(const BufferLayout& layout) override;
	virtual const BufferLayout& layout() const override { return m_layout; }

private:
	BufferLayout	m_layout;
	u32		m_rendererId = 0;
};

class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(u32* indices, u32 size);
	virtual ~OpenGLIndexBuffer();

	virtual void bind() const override;
	virtual void unbind() const override;

	virtual u32 count() const override { return m_indicesCount; }

private:
	u32 m_rendererId = 0;
	u32 m_indicesCount = 0;
};

}