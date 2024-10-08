#pragma once

#include "Journey/Renderer/Framebuffer.h"

namespace jny
{

class OpenGLFramebuffer : public Framebuffer
{
public:
	OpenGLFramebuffer(const FramebufferSpecs& specs);
	virtual ~OpenGLFramebuffer();

	virtual const FramebufferSpecs& specs() const override { return m_specs; }
	virtual FramebufferSpecs& specs() override { return m_specs; }

	virtual void bind() override;
	virtual void unbind() override;

	virtual u32 colorAttachment() override { return m_colorAttachment; }

	virtual void resize(const glm::vec2& size) override;
	void invalidate();

private:
	FramebufferSpecs	m_specs;

	u32			m_colorAttachment = 0;
	u32			m_depthAttachment = 0;
	u32			m_rendererId = 0;
};

}