#pragma once

#include "Journey/Renderer/VertexArray.h"

namespace jny
{

class OpenGLVertexArray : public VertexArray
{
public:
	OpenGLVertexArray();
	virtual ~OpenGLVertexArray();

	virtual void bind() const override;
	virtual void unbind() const override;

	virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
	virtual void setIndexBuffer(const Ref<IndexBuffer>& IndexBuffer) override;

	virtual const std::vector<Ref<VertexBuffer>>& vertexBuffers() const override { return m_vertexBuffers; }
	virtual const Ref<IndexBuffer>& indexBuffer() const override { return m_indexBuffer; }

private:
	std::vector<Ref<VertexBuffer>>	m_vertexBuffers;
	Ref<IndexBuffer>				m_indexBuffer;
	u32						m_vertexBufferIndexOffset = 0;
	u32						m_rendererId;
};

}
