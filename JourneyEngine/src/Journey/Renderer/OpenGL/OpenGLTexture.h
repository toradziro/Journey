#pragma once

#include "Journey/Renderer/Texture.h"

namespace jny
{

class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D(const std::string& texturePath);
	~OpenGLTexture2D();

	virtual uint32_t width() const override;
	virtual uint32_t height() const override;

	virtual void bind(uint32_t slot = 0) const override;

private:
	std::string	m_path;

	uint32_t	m_width = 0;
	uint32_t	m_height = 0;

	uint32_t	m_rendererId = 0;
};

}