#pragma once

#include <glm/glm.hpp>
#include "Journey/ResourceManagers/TextureManager.h"

namespace jny
{

struct QuadCfg;

class Texture
{
public:
	virtual ~Texture() = default;

	virtual u32 width() const = 0;
	virtual u32 height() const = 0;

	virtual void setData(void* data, u32 size) = 0;

	virtual void bind(u32 slot = 0) const = 0;
	virtual u32 rendererId() const = 0;

	virtual std::string path() const = 0;
};

class Texture2D : public Texture
{
public:
	virtual ~Texture2D() = default;

private:
	friend TextureManager;

	static s_ptr<Texture2D> create(const std::string& texturePath);
	static s_ptr<Texture2D> create(u32 width, u32 height);
};

class SubTexture2D
{
public:
	SubTexture2D(const s_ptr<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

	const glm::vec2* textureCoordinates() const { return m_textureCoordinates; }
	const s_ptr<Texture2D>& texture() const { return m_texture; }

	static s_ptr<SubTexture2D> createFromCoords(const glm::vec2& coordinates,
		const glm::vec2& spriteSize,
		const glm::vec2& cellsFromAtlas,
		const s_ptr<Texture2D>& texture);

private:
	s_ptr<Texture2D>	m_texture;

	//-- Offset in a sprite sheet for one index
	glm::vec2		m_textureCoordinates[4];
};

} //--jny
