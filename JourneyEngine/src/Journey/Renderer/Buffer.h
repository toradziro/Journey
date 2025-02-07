#pragma once

#include "Journey/Core/Core.h"

namespace jny
{

namespace buff_utils
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

u32 shaderDataTypeSize(ShaderDataType type);
u32 componentsCount(buff_utils::ShaderDataType type);

}

struct LayoutElement
{
	std::string					m_varName;
	//-- Crossplatfom way of describing layout variable type
	buff_utils::ShaderDataType	m_type = buff_utils::ShaderDataType::None;
	//-- Size in bytes of current layout element - need to collect offset & stride
	u32							m_size;
	//-- Offset from the start of layout to current layout element
	u32							m_offset;
	//-- Amount of elements in a layout - vec3 - 3 floats
	u32							m_count;
	//-- Is normalized data
	bool						m_normilized;

	LayoutElement(buff_utils::ShaderDataType type, const std::string& name, bool normalized = false)
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

class VertexBuffer
{
public:
	virtual ~VertexBuffer() = default;

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual void setData(const void* data, u32 size) = 0;

	virtual void setLayout(const BufferLayout& layout) = 0;
	virtual const BufferLayout& layout() const = 0;

	static s_ptr<VertexBuffer> create(u32 count);
	static s_ptr<VertexBuffer> create(float* vertices, u32 count);
};

class IndexBuffer
{
public:
	virtual ~IndexBuffer() = default;

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual u32 count() const = 0;

	static s_ptr<IndexBuffer> create(u32* indices, u32 count);
};

} //-- jny