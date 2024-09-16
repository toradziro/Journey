#pragma once

#include "Journey/Core/Reference.h"
#include "Journey/Core/SingletonInterface.h"
#include "Journey/Renderer/OrthographicCamera.h"
#include "Journey/Renderer/VertexArray.h"
#include "Journey/Renderer/Shader.h"
#include "Journey/Renderer/Texture.h"

namespace jny
{

enum class TextureOpt : uint8_t
{
	FlatColored = 0,
	Textured,
};

enum class RotateOpt : uint8_t
{
	Rotated = 0,
	AlignedByAxices
};

struct QuadCfg
{
	glm::vec4		m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec3		m_position = { 1.0f, 1.0f, 0.0f };
	glm::vec3		m_size = { 1.0f, 1.0f, 0.0f };
	float			m_rotation = 0.0f;
	float			m_rotationDegrees = 0.0f;
	float			m_tilingFactor = 1.0f;
	Ref<Texture2D>	m_texture = nullptr;

	TextureOpt		m_textureOpt = TextureOpt::FlatColored;
	RotateOpt		m_rotateOpt = RotateOpt::AlignedByAxices;
};

class Renderer2D : public ISingleton
{
	JNY_SINGLETON_TYPE(Renderer2D)
public:
	void init();
	void shutdown();

	void beginScene(const OrthographicCamera& camera);
	void endScene();

	//-- Primitives
	void drawQuad(const QuadCfg& cfg);

private:
	Ref<VertexArray>	m_quadVertexArray;
	Ref<Shader>			m_textureShader;
	Ref<Texture2D>		m_whiteTexture;
};

} //-- jny