#pragma once

#include <glm/glm.hpp>

namespace jny
{

struct QuadCfg;

void sampleTexture(QuadCfg& cfg, glm::vec2 sampledTexture);

class Texture : public ReferenceCounter
{
public:
	virtual ~Texture() = default;

	virtual uint32_t width() const = 0;
	virtual uint32_t height() const = 0;

	virtual void setData(void* data, uint32_t size) = 0;

	virtual void bind(uint32_t slot = 0) const = 0;
	virtual uint32_t rendererId() const = 0;
};

class Texture2D : public Texture
{
public:
	virtual ~Texture2D() = default;

	static Ref<Texture2D> create(const std::string& texturePath);
	static Ref<Texture2D> create(uint32_t width, uint32_t height);
};

class SubTexture2D : public ReferenceCounter
{
public:
	SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

	const glm::vec2* textureCoordinates() const { return m_textureCoordinates; }
	const Ref<Texture2D>& texture() const { return m_texture; }

	static Ref<SubTexture2D> createFromCoords(const glm::vec2& coordinates, const glm::vec2& spriteSize, const Ref<Texture2D>& texture);

private:
	Ref<Texture2D>	m_texture;

	//-- Offset in a sprite sheet for one index
	//glm::vec2		m_offset; 
	glm::vec2		m_textureCoordinates[4];
};

} //--jny
