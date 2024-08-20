#pragma once

#include "Journey/Renderer/Buffer.h"

namespace jny
{

class VertexArray : public ReferenceCounter
{
public:
	virtual ~VertexArray() = default;

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
	virtual void setIndexBuffer(const Ref<IndexBuffer>& IndexBuffer) = 0;

	virtual const std::vector<Ref<VertexBuffer>>& vertexBuffers() const = 0;
	virtual const Ref<IndexBuffer>& indexBuffer() const = 0;

	static VertexArray* create();
};

} //-- jny