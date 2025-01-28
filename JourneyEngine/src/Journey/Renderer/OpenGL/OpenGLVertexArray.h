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

	virtual void addVertexBuffer(const s_ptr<VertexBuffer>& vertexBuffer) override;
	virtual void setIndexBuffer(const s_ptr<IndexBuffer>& IndexBuffer) override;

	virtual const std::vector<s_ptr<VertexBuffer>>& vertexBuffers() const override { return m_vertexBuffers; }
	virtual const s_ptr<IndexBuffer>& indexBuffer() const override { return m_indexBuffer; }

private:
	std::vector<s_ptr<VertexBuffer>>	m_vertexBuffers;
	s_ptr<IndexBuffer>				m_indexBuffer;
	u32								m_vertexBufferIndexOffset = 0;
	u32								m_rendererId;
};

}
