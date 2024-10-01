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

	virtual uint32_t colorAttachment() override { return m_colorAttachment; }

	virtual void resize() override;

private:
	FramebufferSpecs	m_specs;

	uint32_t			m_colorAttachment = 0;
	uint32_t			m_depthAttachment = 0;
	uint32_t			m_rendererId = 0;
};

}