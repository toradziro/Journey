#pragma once

#include "Journey/Core/Core.h"

namespace jny
{

enum class ShaderDataType : u8
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

u32 shaderDataTypeSize(ShaderDataType type)
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

	JNY_ASSERT(false, "Invalid Type");
	return 0;
}

u32 componentsCount(ShaderDataType type)
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

	JNY_ASSERT(false, "Invalid type");
	return 0;
}

} //-- unnamed

struct LayoutElement
{
	std::string		m_varName;
	//-- Crossplatfom way of describing layout variable type
	ShaderDataType	m_type = ShaderDataType::None;
	//-- Size in bytes of current layout element - need to collect offset & stride
	u32		m_size;
	//-- Offset from the start of layout to current layout element
	u32		m_offset;
	//-- Amount of elements in a layout - vec3 - 3 floats
	u32		m_count;
	//-- Is normalized data
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
	using LayoutData = std::vector<LayoutElement>;
	using LayoutDataIterator = std::vector<LayoutElement>::iterator;
	using LayoutDataConstIterator = std::vector<LayoutElement>::const_iterator;

	BufferLayout() = default;
	BufferLayout(LayoutData&& elements) : m_elements(std::move(elements))
	{
		calculateOffsetAndStride();
	}
	~BufferLayout() = default;

	inline const LayoutData& elements() const { return m_elements; }
	inline u32 stride() const { return m_stride; }

	LayoutDataIterator begin() { return m_elements.begin(); }
	LayoutDataIterator end() { return m_elements.end(); }

	LayoutDataConstIterator begin() const { return m_elements.cbegin(); }
	LayoutDataConstIterator end() const { return m_elements.cend(); }

private:
	void calculateOffsetAndStride()
	{
		u32 offset = 0;
		
		std::ranges::for_each(m_elements, [&offset](auto& element)
			{
				element.m_offset = offset;
				offset = offset + element.m_size;
			});
		//-- All elements already calculated full stride size in offset
		m_stride = offset;
	}

	//-- All layouts set for VertexArray
	LayoutData m_elements;
	//-- Full size of layout in bytes
	u32 m_stride = 0;
};

class VertexBuffer : public ReferenceCounter
{
public:
	virtual ~VertexBuffer() = default;

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual void setData(const void* data, u32 size) = 0;

	virtual void setLayout(const BufferLayout& layout) = 0;
	virtual const BufferLayout& layout() const = 0;

	static VertexBuffer* create(u32 count);
	static VertexBuffer* create(float* vertices, u32 count);
};

class IndexBuffer : public ReferenceCounter
{
public:
	virtual ~IndexBuffer() = default;

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual u32 count() const = 0;

	static IndexBuffer* create(u32* indices, u32 count);
};

} //-- jny