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
	constexpr static	uint32_t C_MAX_QUADS_IN_A_BATCH = 10000;
	constexpr static	uint32_t C_MAX_TEXTURE_SLOTS = 32;
	constexpr static	uint32_t C_VERTICES_IN_QUAD = 4;
	constexpr static	uint32_t C_INDICES_IN_QUAD = 6;

	void init();
	void shutdown();

	void beginScene(const OrthographicCamera& camera);
	void endScene();

	void flush();

	//-- Primitives
	void drawQuad(const QuadCfg& cfg);
	
	//-- Statistics
	void resetStatistics() { m_frameStat = {}; }
	const auto& stats() const { return m_frameStat; }

private:
	void startNextBatch();

private:
	struct QuadVertex
	{
		glm::vec3	m_position = {};
		glm::vec4	m_color = {};
		glm::vec2	m_textureCoordinate = {};
		float		m_textureIndex = 0.0f;
		float		m_tilingFactor = 0.0f;
	};

	struct Statistic
	{
		uint32_t	m_drawCalls = 0;
		uint32_t	m_quadCount = 0;

		uint32_t vertexCount() const { return m_quadCount * C_VERTICES_IN_QUAD; }
		uint32_t indexCount() const { return m_quadCount * C_INDICES_IN_QUAD; }
	};

	std::array<Ref<Texture2D>, C_MAX_TEXTURE_SLOTS>	m_textureSlots;
	const OrthographicCamera*						m_currCamera;
	//-- Need to apply transformation matrix to
	glm::vec4										m_quadVertexPosition[C_VERTICES_IN_QUAD];

	Statistic										m_frameStat;

	Ref<VertexArray>								m_quadVertexArray;
	Ref<VertexBuffer>								m_quadVertexBuffer;
	Ref<Shader>										m_textureShader;
	Ref<Texture2D>									m_whiteTexture;

	QuadVertex*										m_quadVertexBase = nullptr;
	QuadVertex*										m_quadVertexPtr = nullptr;

	uint32_t										m_currQuadIndex = 0;
	uint32_t										m_currTextureSlot = 1; //-- 0 is for white texture
};

} //-- jny