#pragma once

#include "Journey/Renderer/Texture.h"
#include <GLAD/glad.h>

namespace jny
{

class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D(const std::string& texturePath);
	OpenGLTexture2D(u32 width, u32 height);
	~OpenGLTexture2D();

	virtual u32 width() const override;
	virtual u32 height() const override;
	virtual u32 rendererId() const override { return m_rendererId; }

	virtual void setData(void* data, u32 size) override;

	virtual void bind(u32 slot = 0) const override;
	virtual std::string path() const override { return m_path; }

private:
	std::string	m_path;

	u32	m_width = 0;
	u32	m_height = 0;

	u32	m_rendererId = 0;

	GLenum		m_internalFormat;
	GLenum		m_dataFormat;
};

}