#pragma once

#include <glm/glm.hpp>

namespace jny
{

enum class FrambufferTextureFormat
{
	None = 0,

	//-- Color
	RGBA8,
	RED_INTEGER,

	//-- Depth/stencil
	DEPTH24STENCIL8,

	Depth = DEPTH24STENCIL8
};

struct FrambufferTextureSpecification
{
	FrambufferTextureSpecification() = default;
	FrambufferTextureSpecification(FrambufferTextureFormat tFormat) :
		m_textureFormat(tFormat)
	{ }

	FrambufferTextureFormat m_textureFormat;
};

struct FrambufferTextureDescription
{
	FrambufferTextureDescription() = default;
	FrambufferTextureDescription(std::initializer_list<FrambufferTextureSpecification> specs) :
		m_textureSpecs(specs)
	{ }

	std::vector<FrambufferTextureSpecification> m_textureSpecs;
};

struct FramebufferSpecs
{
	FrambufferTextureDescription	m_textureDescription;

	u32								m_width = 1;
	u32								m_height = 1;
	u32								m_samples = 1;

	//-- rendering on the screen or not
	bool							m_swapChainTarget = false;
};

class Framebuffer : public ReferenceCounter
{
public:
	virtual ~Framebuffer() = default;

	virtual const FramebufferSpecs& specs() const = 0;
	virtual FramebufferSpecs& specs() = 0;

	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void resize(const glm::vec2& size) = 0;

	virtual u32 colorAttachment(u32 index = 0) = 0;
	virtual int readbackPixel(u32 index, i32 x, i32 y) = 0;

	static Ref<Framebuffer> create(const FramebufferSpecs& specs);
};

}