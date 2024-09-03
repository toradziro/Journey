#include "jnypch.h"
#include "OpenGLTexture.h"
#include "Journey/Core/Core.h"

#include "stb_image.h"
#include <GLAD/glad.h>

namespace jny
{

OpenGLTexture2D::OpenGLTexture2D(const std::string& texturePath)
	: m_path(texturePath)
{
	int32_t width = 0;
	int32_t height = 0;
	int32_t channelsInFile = 0;

	//-- stbi_uc is a read image
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = stbi_load(texturePath.c_str(), &width, &height, &channelsInFile, 0);
	JNY_ASSERT(data, "failed load texture");

	m_width = width;
	m_height = height;

	//-- For internal format we need to set bits per channel to know how deep color is
	GLenum internalFormat = channelsInFile == 3 ? GL_RGB8 : GL_RGBA8;
	//-- While for data format we need to know only how it's mixed together
	GLenum format = channelsInFile == 3 ? GL_RGB : GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
	//-- Allocation of the texture in GPU memory
	glTextureStorage2D(m_rendererId, 1, internalFormat, m_width, m_height);

	//-- Parameters about how texture will be scaled during rendering if
	//-- our object coordinates will be not in 1x1 space
	glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, format, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &m_rendererId);
}

uint32_t OpenGLTexture2D::width() const
{
	return m_width;
}

uint32_t OpenGLTexture2D::height() const
{
	return m_height;
}

void OpenGLTexture2D::bind(uint32_t slot) const
{
	glBindTextureUnit(slot, m_rendererId);
}

} //-- jny