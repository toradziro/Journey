#pragma once

#include <glm/glm.hpp>

namespace jny
{

struct FramebufferSpecs
{
	u32	m_width = 0;
	u32	m_height = 0;
	u32	m_samples = 1;

	//-- rendering on the screen or not
	bool		m_swapChainTarget = false;
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

	virtual u32 colorAttachment() = 0;

	static Ref<Framebuffer> create(const FramebufferSpecs& specs);
};

}