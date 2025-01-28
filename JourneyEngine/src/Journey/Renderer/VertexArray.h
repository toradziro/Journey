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

	virtual void addVertexBuffer(const s_ptr<VertexBuffer>& vertexBuffer) = 0;
	virtual void setIndexBuffer(const s_ptr<IndexBuffer>& IndexBuffer) = 0;

	virtual const std::vector<s_ptr<VertexBuffer>>& vertexBuffers() const = 0;
	virtual const s_ptr<IndexBuffer>& indexBuffer() const = 0;

	static VertexArray* create();
};

} //-- jny