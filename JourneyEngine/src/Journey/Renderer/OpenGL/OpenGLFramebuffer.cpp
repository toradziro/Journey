#include "jnypch.h"
#include "OpenGLFramebuffer.h"
#include "Journey/Core/Core.h"
#include <GLAD/glad.h>

namespace jny
{

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecs& specs) :
	m_specs(specs)
{
	resize();
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
	glDeleteFramebuffers(1, &m_rendererId);
}

void OpenGLFramebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId);
}

void OpenGLFramebuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::resize()
{
	glCreateFramebuffers(1, &m_rendererId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId);
	
	glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachment);
	glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specs.m_width, m_specs.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_specs.m_width, m_specs.m_height,
		0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

	JNY_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer failed");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}