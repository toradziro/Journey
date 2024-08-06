#pragma once

namespace jny
{

class VertexBuffer
{
public:
	virtual ~VertexBuffer() {}

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	static VertexBuffer* create(float* vertices, uint32_t count);
};

class IndexBuffer
{
public:
	virtual ~IndexBuffer() {}

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual uint32_t count() const = 0;

	static IndexBuffer* create(uint32_t* indices, uint32_t count);
};

} //-- jny