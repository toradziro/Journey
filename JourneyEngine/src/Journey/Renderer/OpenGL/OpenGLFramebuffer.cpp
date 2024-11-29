#include "jnypch.h"
#include "OpenGLFramebuffer.h"
#include "Journey/Core/Core.h"

#include <GLAD/glad.h>
#include <glm/glm.hpp>

namespace jny
{

static bool isDepthFormat(FrambufferTextureFormat format)
{
	switch (format)
	{
	case FrambufferTextureFormat::None:
		return false;
	case FrambufferTextureFormat::RGBA8:
		return false;
	case FrambufferTextureFormat::DEPTH24STENCIL8:
		return true;
	default:
		break;
	}
	return false;
}

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecs& specs) :
	m_specs(specs)
{
	for (const auto& att : m_specs.m_textureDescription.m_textureSpecs)
	{
		if (isDepthFormat(att.m_textureFormat))
		{
			m_depthBufferFormat = att.m_textureFormat;
		}
		else
		{
			m_colorAttachmentsFormats.push_back(att.m_textureFormat);
		}
	}

	invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
	glDeleteFramebuffers(1, &m_rendererId);
	glDeleteTextures(static_cast<GLsizei>(m_colorAttachments.size()), m_colorAttachments.data());
	glDeleteTextures(1, &m_depthAttachment);
}

void OpenGLFramebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId);
	glViewport(0, 0, m_specs.m_width, m_specs.m_height);
}

void OpenGLFramebuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::resize(const glm::vec2& size)
{
	m_specs.m_width = static_cast<u32>(size.x);
	m_specs.m_height = static_cast<u32>(size.y);

	invalidate();
}

void OpenGLFramebuffer::invalidate()
{
	JNY_ASSERT(m_colorAttachments.size() < 4, "Support only 4 multiple render targets");

	if (m_rendererId != 0)
	{
		glDeleteFramebuffers(1, &m_rendererId);
		glDeleteTextures(static_cast<GLsizei>(m_colorAttachments.size()), m_colorAttachments.data());
		glDeleteTextures(1, &m_depthAttachment);

		m_colorAttachments.clear();
		m_depthAttachment = 0;
	}

	glCreateFramebuffers(1, &m_rendererId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId);

	//-- Create and attach color textures
	createColorTextures();
	//-- Depth buffer also comes here
	createDepthTexture();

	if (m_colorAttachments.size() > 1)
	{
		GLenum renderTargets[4] =
		{
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_COLOR_ATTACHMENT3
		};

		glDrawBuffers(static_cast<GLsizei>(m_colorAttachments.size()), renderTargets);
	}
	else if (m_colorAttachments.empty())
	{
		glDrawBuffer(GL_NONE);
	}

	JNY_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer failed");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::createColorTextures()
{
	//-- Attachments
	if (m_colorAttachmentsFormats.size() > 0)
	{
		m_colorAttachments.resize(m_colorAttachmentsFormats.size());
		glCreateTextures(GL_TEXTURE_2D,
			static_cast<GLsizei>(m_colorAttachmentsFormats.size()),
			m_colorAttachments.data());

		for (u32 i = 0; i < m_colorAttachments.size(); ++i)
		{
			glBindTexture(GL_TEXTURE_2D, m_colorAttachments[i]);
			//-- Internal format is a format on gpu side
			//-- Format is a format of data we send from RAM to VRAM
			//-- i.e. which data we have and how we want it to be treated on GPU side
			GLint internalFormat = 0;
			GLenum format = 0;
			//-- Convert formats, maybe need to move in another function
			switch (m_colorAttachmentsFormats[i])
			{
			case FrambufferTextureFormat::RGBA8:
				format = GL_RGBA;
				internalFormat = GL_RGBA8;
				break;
			default:
				JNY_ASSERT(false, "");
				break;
			}

			//-- Allocate memory for texture
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				internalFormat,
				m_specs.m_width,
				m_specs.m_height,
				0,
				format,
				GL_UNSIGNED_BYTE,
				nullptr);

			//-- Filtering
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glFramebufferTexture2D(GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0 + i,
				GL_TEXTURE_2D,
				m_colorAttachments[i],
				0);
		}
	}
}

void OpenGLFramebuffer::createDepthTexture()
{
	if (m_depthBufferFormat != FrambufferTextureFormat::None)
	{
		GLint internalFormat = 0;
		GLenum format = 0;
		switch (m_depthBufferFormat)
		{
			case FrambufferTextureFormat::DEPTH24STENCIL8:
				format = GL_DEPTH_STENCIL;
				internalFormat = GL_DEPTH24_STENCIL8;
				break;
			default:
				JNY_ASSERT(false, "");
				break;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			internalFormat,
			m_specs.m_width,
			m_specs.m_height,
			0,
			format,
			GL_UNSIGNED_INT_24_8,
			nullptr);

		glFramebufferTexture2D(GL_FRAMEBUFFER,
			GL_DEPTH_STENCIL_ATTACHMENT,
			GL_TEXTURE_2D,
			m_depthAttachment,
			0);
	}
}

}