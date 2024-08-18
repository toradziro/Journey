#pragma once

#include "Journey/Renderer/Buffer.h"

namespace jny
{

class VertexArray
{
public:
	virtual ~VertexArray() = default;

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
	virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& IndexBuffer) = 0;

	virtual const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers() const = 0;
	virtual const std::shared_ptr<IndexBuffer>& indexBuffer() const = 0;

	static VertexArray* create();
};

} //-- jny