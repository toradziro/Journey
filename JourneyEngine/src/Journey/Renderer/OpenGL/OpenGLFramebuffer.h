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

	virtual u32 colorAttachment(u32 index = 0) override { return m_colorAttachments[0]; }

	virtual void resize(const glm::vec2& size) override;
	void invalidate();

private:
	void createColorTextures();
	void createDepthTexture();

private:
	FramebufferSpecs	m_specs;

	u32					m_depthAttachment = 0;
	u32					m_rendererId = 0;

	std::vector<FrambufferTextureFormat>	m_colorAttachmentsFormats;
	FrambufferTextureFormat					m_depthBufferFormat = FrambufferTextureFormat::None;

	std::vector<u32>	m_colorAttachments;
};

}