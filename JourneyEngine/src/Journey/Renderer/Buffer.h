#pragma once

#include "Journey/Core.h"
#include <glad/glad.h>

namespace jny
{

enum class ShaderDataType : uint8_t
{
	Bool,
	Int,
	Int2,
	Int3,
	Int4,

	Float,
	Float2,
	Float3,
	Float4,

	Mat3,
	Mat4,

	None
};

namespace
{

uint32_t shaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Bool:		return sizeof(bool);
	case ShaderDataType::Int:		return sizeof(int);
	case ShaderDataType::Int2:		return sizeof(int) * 2;
	case ShaderDataType::Int3:		return sizeof(int) * 3;
	case ShaderDataType::Int4:		return sizeof(int) * 4;
	case ShaderDataType::Float:		return sizeof(float);
	case ShaderDataType::Float2:	return sizeof(float) * 2;
	case ShaderDataType::Float3:	return sizeof(float) * 3;
	case ShaderDataType::Float4:	return sizeof(float) * 4;
	//-- 3x3 matrix
	case ShaderDataType::Mat3:		return sizeof(float) * 3 * 3;
	//-- 4x4 matrix
	case ShaderDataType::Mat4:		return sizeof(float) * 4 * 4;
	default:
		break;
	}

	JNY_ASSERT(false);
	return 0;
}

uint32_t componentsCount(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Bool:		return 1;
	case ShaderDataType::Int:		return 1;
	case ShaderDataType::Int2:		return 2;
	case ShaderDataType::Int3:		return 3;
	case ShaderDataType::Int4:		return 4;
	case ShaderDataType::Float:		return 1;
	case ShaderDataType::Float2:	return 2;
	case ShaderDataType::Float3:	return 3;
	case ShaderDataType::Float4:	return 4;
	//-- 3x3 matrix
	case ShaderDataType::Mat3:		return 3 * 3;
	//-- 4x4 matrix
	case ShaderDataType::Mat4:		return 4 * 4;
	default:
		break;
	}

	JNY_ASSERT(false);
	return 0;
}

} //-- unnamed

struct LayoutElement
{
	std::string		m_varName;
	ShaderDataType	m_type = ShaderDataType::None;
	uint32_t		m_size;
	uint32_t		m_offset;
	uint32_t		m_count;
	bool			m_normilized;

	LayoutElement(ShaderDataType type, const std::string& name, bool normalized = false)
		: m_varName(name),
		m_type(type),
		m_size(shaderDataTypeSize(type)),
		m_offset(0),
		m_count(componentsCount(type)),
		m_normilized(normalized)
	{}
};

class BufferLayout
{
public:
	using LayoutData = eastl::vector<LayoutElement>;
	using LayoutDataIterator = eastl::vector<LayoutElement>::iterator;

	BufferLayout() = default;
	BufferLayout(LayoutData&& elements) : m_elements(std::move(elements))
	{
		calculateOffsetAndStride();
	}
	~BufferLayout() {}

	inline const LayoutData& elements() const { return m_elements; }
	inline const uint32_t stride() const { return m_stride; }

	LayoutDataIterator begin() { return m_elements.begin(); }
	LayoutDataIterator end() { return m_elements.end(); }

private:
	void calculateOffsetAndStride()
	{
		uint32_t offset = 0;
		
		for (auto& element : m_elements)
		{
			element.m_offset = offset;
			offset = offset + element.m_size;
		}
		//-- All elements already calculated full stride size in offset
		m_stride = offset;
	}

	LayoutData m_elements;
	uint32_t m_stride = 0;
};

class VertexBuffer
{
public:
	virtual ~VertexBuffer() {}

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual void setLayout(const BufferLayout& layout) = 0;
	virtual const BufferLayout& layout() const = 0;

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