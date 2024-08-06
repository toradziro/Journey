#pragma once

namespace jny
{

class VertexBuffer
{
public:
	virtual ~VertexBuffer() {}

	virtual void bind() = 0;
	virtual void unbind() = 0;

	static VertexBuffer* create(float* vertices, uint32_t size);
};

class IndexBuffer
{
public:
	virtual ~IndexBuffer() {}

	virtual void bind() = 0;
	virtual void unbind() = 0;

	static IndexBuffer* create(uint32_t* indices, uint32_t size);
};

} //-- jny