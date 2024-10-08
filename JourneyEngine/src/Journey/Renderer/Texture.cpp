#include "jnypch.h"
#include "Texture.h"

#include "Journey/Renderer/Renderer.h"
#include "Journey/Renderer/OpenGL/OpenGLTexture.h"
#include "Journey/Core/Application.h"
#include "Journey/Renderer/Renderer2D.h"

namespace jny
{

void sampleTexture(QuadCfg& cfg, glm::vec2 sampledTexture, const glm::vec2& spriteSize, const glm::vec2& cellsFromAtlas)
{
	Ref<SubTexture2D> sbt = SubTexture2D::createFromCoords(sampledTexture, spriteSize, cellsFromAtlas, cfg.m_texture);

	for (int i = 0; i < 4; ++i)
	{
		cfg.m_texturesPos[i] = sbt->textureCoordinates()[i];
	}
}

Ref<Texture2D> Texture2D::create(const std::string& texturePath)
{
	auto rendererApi = Application::subsystems().st<Renderer>().API();
	Ref<Texture2D> tex = nullptr;

	switch (rendererApi)
	{
	case RendererAPI::API::None:
		JNY_ASSERT(true, "Can't be None");
		break;
	case RendererAPI::API::OpenGL:
		tex = new OpenGLTexture2D(texturePath);
		break;
	default:
		break;
	}

	return tex;
}

Ref<Texture2D> Texture2D::create(u32 width, u32 height)
{
	auto rendererApi = Application::subsystems().st<Renderer>().API();
	Ref<Texture2D> tex = nullptr;

	switch (rendererApi)
	{
	case RendererAPI::API::None:
		JNY_ASSERT(true, "Can't be None");
		break;
	case RendererAPI::API::OpenGL:
		tex = new OpenGLTexture2D(width, height);
		break;
	default:
		break;
	}

	return tex;
}

SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
	: m_texture(texture)
{
	m_textureCoordinates[0] = min;
	m_textureCoordinates[1] = { max.x, min.y };
	m_textureCoordinates[2] = max;
	m_textureCoordinates[3] = { min.x, max.y };
}

Ref<SubTexture2D> SubTexture2D::createFromCoords(const glm::vec2& coordinates,
	const glm::vec2& spriteSize,
	const glm::vec2& cellsFromAtlas,
	const Ref<Texture2D>& texture)
{
	float width = static_cast<float>(texture->width());
	float height = static_cast<float>(texture->height());

	float startPosX = (spriteSize.x * coordinates.x) / width;
	float startPosY = (spriteSize.y * coordinates.y) / height;

	//-- cellsFromAtlas we need to find out how much textures we want to sample, i.e. if we want tree divided on 2 textures be drawn in one call
	float stepX = (spriteSize.x / width) * cellsFromAtlas.x;
	float stepY = (spriteSize.y / height) * cellsFromAtlas.y;

	glm::vec2 minCoords = { startPosX, startPosY };
	glm::vec2 maxCoords = { startPosX + stepX, startPosY + stepY };
	return Ref<SubTexture2D>::create(texture, minCoords, maxCoords);
}

} //-- jny