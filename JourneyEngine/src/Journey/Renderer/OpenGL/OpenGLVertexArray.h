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

	virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
	virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& IndexBuffer) override;

	virtual const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers() const override { return m_vertexBuffers; }
	virtual const std::shared_ptr<IndexBuffer>& indexBuffer() const override { return m_indexBuffer; }

private:
	std::vector<std::shared_ptr<VertexBuffer>>	m_vertexBuffers;
	std::shared_ptr<IndexBuffer>				m_indexBuffer;
	uint32_t									m_rendererId;
};

}
