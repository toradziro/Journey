#pragma once

#include "RenderCommand.h"
#include "Journey/Core/Reference.h"
#include "Journey/Core/SingletonInterface.h"
#include "Journey/Renderer/Camera.h"
#include "Journey/Renderer/VertexArray.h"
#include "Journey/Renderer/Shader.h"
#include "Journey/Renderer/Texture.h"
#include "Journey/Scene/Components.h"

namespace jny
{

enum class TextureOpt : u8
{
	FlatColored = 0,
	Textured,
};

enum class RotateOpt : u8
{
	Rotated = 0,
	AlignedByAxices
};

struct QuadCfg
{
	glm::mat4		m_transform = { 1.0f };

	glm::vec4		m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	f32				m_zDepth = 0.0f;
	f32				m_tilingFactor = 1.0f;
	i32				m_entityId = -1;
	glm::vec2		m_texturesPos[4] = {
								{ 0.0f, 0.0f },
								{ 1.0f, 0.0f },
								{ 1.0f, 1.0f },
								{ 0.0f, 1.0f }
							};
	s_ptr<Texture2D>	m_texture = nullptr;

	TextureOpt		m_textureOpt = TextureOpt::FlatColored;
	RotateOpt		m_rotateOpt = RotateOpt::AlignedByAxices;
};

struct CircleCfg
{
	glm::mat4		m_transform = { 1.0f };
	glm::vec4		m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	f32				m_zDepth = 0.0f;
	f32				m_radius = 0.0f;
	f32				m_thikness = 0.0f;

	i32				m_entityId = -1;
};

struct LineCfg
{
	glm::vec4		m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec3		m_startPoint = {};
	glm::vec3		m_endPoint = {};

	i32				m_entityId = -1;
};

struct RectangleCfg
{
	glm::mat4		m_transform = { 1.0f };
	glm::vec4		m_color = { 1.0f, 1.0f, 1.0f, 1.0f };

	i32				m_entityId = -1;
};

class Renderer2D : public ISingleton
{
	JNY_SINGLETON_TYPE(Renderer2D)
public:
	constexpr static	u32 C_MAX_QUADS_IN_A_BATCH = 10000;
	constexpr static	u32 C_MAX_TEXTURE_SLOTS = 32;
	constexpr static	u32 C_VERTICES_IN_QUAD = 4;
	constexpr static	u32 C_INDICES_IN_QUAD = 6;

	void			init();
	void			shutdown();

	void			beginScene(const EditorCamera& camera);
	void			beginScene(const CameraComponent& camera, const glm::mat4& transform);
	void			endScene();

	void			flushQuads();
	void			flushCircles();
	void			flushLines();

	//-- Primitives
	void			drawQuad(const QuadCfg& cfg);
	void			drawCircle(const CircleCfg& cfg);
	void			drawLine(const LineCfg& cfg);
	void			drawRectangle(const RectangleCfg& cfg);
	
	//-- Statistics
	void			resetStatistics() { m_frameStat = {}; }
	const auto&		stats() const { return m_frameStat; }

	void			windowResized(u32 width, u32 height);

private:
	void			startNextBatch();
	void			prepareQuadsDrawer();
	void			prepareCircleDrawer();
	void			prepareLineDrawer();

private:
	struct QuadVertex
	{
		glm::vec3	m_position = {};
		glm::vec4	m_color = {};
		glm::vec2	m_textureCoordinate = {};
		f32			m_textureIndex = 0.0f;
		f32			m_tilingFactor = 0.0f;
		i32			m_entityId = -1;
	};

	struct CircleVertex
	{
		glm::vec3	m_position = {};
		glm::vec4	m_color = {};
		f32			m_radius = 0.0f;
		f32			m_thikness = 0.0f;
		glm::vec2	m_localPos = {};
		i32			m_entityId = -1;
	};

	struct LineVertex
	{
		glm::vec3	m_position = {};
		glm::vec4	m_color = {};
		i32			m_entityId = -1;
	};

	struct Statistic
	{
		u32	m_drawCalls = 0;
		u32	m_quadCount = 0;

		u32 vertexCount() const { return m_quadCount * C_VERTICES_IN_QUAD; }
		u32 indexCount() const { return m_quadCount * C_INDICES_IN_QUAD; }
	};

	std::array<s_ptr<Texture2D>, C_MAX_TEXTURE_SLOTS>	m_textureSlots;
	//-- Need to apply transformation matrix to
	glm::vec4										m_quadVertexPosition[C_VERTICES_IN_QUAD];

	glm::mat4										m_currentFrameViewProjection = { 1.0f };

	Statistic										m_frameStat;

	s_ptr<VertexArray>								m_quadVertexArray;
	s_ptr<VertexArray>								m_circleVertexArray;
	s_ptr<VertexArray>								m_lineVertexArray;

	s_ptr<VertexBuffer>								m_quadVertexBuffer;
	s_ptr<VertexBuffer>								m_circleVertexBuffer;
	s_ptr<VertexBuffer>								m_lineVertexBuffer;

	s_ptr<Shader>									m_quadShader;
	s_ptr<Shader>									m_circleShader;
	s_ptr<Shader>									m_lineShader;

	s_ptr<Texture2D>								m_whiteTexture;

	QuadVertex*										m_quadVertexBase = nullptr;
	QuadVertex*										m_quadVertexPtr = nullptr;

	CircleVertex*									m_circleVertexBase = nullptr;
	CircleVertex*									m_circleVertexPtr = nullptr;

	LineVertex*										m_lineVertexBase = nullptr;
	LineVertex*										m_lineVertexPtr = nullptr;

	u32												m_currQuadBatchIndex = 0;
	u32												m_currCircleBatchIndex = 0;
	u32												m_currLineBatchIndex = 0;
	u32												m_currTextureSlot = 1; //-- 0 is for white texture
};

} //-- jny