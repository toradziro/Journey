#include "jnypch.h"
#include "Renderer2D.h"
#include "Journey/Renderer/VertexArray.h"
#include "Journey/Renderer/Shader.h"
#include "Journey/Renderer/RenderCommand.h"
#include "Journey/Core/Application.h"
#include "Journey/Core/Profiling/TimeInstruments.h"
#include "Journey/ResourceManagers/TextureManager.h"

#include <glm/gtc/matrix_transform.hpp>

namespace
{

constexpr	u32 C_MAX_VERTICES = jny::Renderer2D::C_MAX_QUADS_IN_A_BATCH * jny::Renderer2D::C_VERTICES_IN_QUAD;
constexpr	u32 C_MAX_INDICES = jny::Renderer2D::C_MAX_QUADS_IN_A_BATCH * jny::Renderer2D::C_INDICES_IN_QUAD;
constexpr	u32 C_INVALID_INDEX = std::numeric_limits<u32>::max();

u32* generateIndices()
{
	u32* indices = new u32[C_MAX_INDICES];

	u32 offset = 0;
	for (u32 i = 0; i < C_MAX_INDICES - 5; i += jny::Renderer2D::C_INDICES_IN_QUAD)
	{
		indices[i] = offset;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;

		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset;

		offset += jny::Renderer2D::C_VERTICES_IN_QUAD;
	}
	return indices;
}

} //-- unnamed

namespace jny
{

void Renderer2D::prepareQuadsDrawer()
{
	//-- Vertex array
	m_quadVertexArray = VertexArray::create();

	//-- Vertex buffer
	m_quadVertexBuffer = VertexBuffer::create(C_MAX_VERTICES * sizeof(QuadVertex));

	//-- Setting up vertex attribute array (layout for providing data splitting in shader)
	BufferLayout::LayoutData layoutData = {
		{ buff_utils::ShaderDataType::Float3, "a_position" },
		{ buff_utils::ShaderDataType::Float4, "a_color" },
		{ buff_utils::ShaderDataType::Float2, "a_texturePos" },
		{ buff_utils::ShaderDataType::Float, "a_textureIndex" },
		{ buff_utils::ShaderDataType::Float, "a_tilingFactor" },
		{ buff_utils::ShaderDataType::Int, "a_entityId" }
	};
	BufferLayout layout = BufferLayout(std::move(layoutData));
	m_quadVertexBuffer->setLayout(layout);
	m_quadVertexArray->addVertexBuffer(m_quadVertexBuffer);

	m_quadVertexBase = new QuadVertex[C_MAX_VERTICES];

	//-- Indices & Index buffer
	u32* indices = generateIndices();

	s_ptr<IndexBuffer> indexBuffer = s_ptr<IndexBuffer>(IndexBuffer::create(indices, C_MAX_INDICES));
	m_quadVertexArray->setIndexBuffer(indexBuffer);
	delete[] indices;

	auto& shaderLib = Application::subsystems().st<ShaderLibrary>();
	m_quadShader = shaderLib.load("assets/shaders/Texture.glsl");
	m_quadShader->bind();

	auto& textureManager = Application::subsystems().st<TextureManager>();
	m_whiteTexture = textureManager.create("white_texture", 1, 1);
	u32 whiteTextureData = 0xffffffff;
	m_whiteTexture->setData(static_cast<void*>(&whiteTextureData), sizeof(whiteTextureData));
	m_textureSlots[0] = m_whiteTexture;

	i32 samplers[C_MAX_TEXTURE_SLOTS]{};
	for (u32 i = 0; i < C_MAX_TEXTURE_SLOTS; ++i)
	{
		samplers[i] = i;
	}
	m_quadShader->uploadUniformIntArray(samplers, C_MAX_TEXTURE_SLOTS, "u_textures");

	m_quadVertexPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	m_quadVertexPosition[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	m_quadVertexPosition[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
	m_quadVertexPosition[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
}

void Renderer2D::prepareCircleDrawer()
{
	//-- Vertex array
	m_circleVertexArray = VertexArray::create();

	//-- Vertex buffer
	m_circleVertexBuffer = VertexBuffer::create(C_MAX_VERTICES * sizeof(CircleVertex));

	BufferLayout::LayoutData layoutData = {
		{ buff_utils::ShaderDataType::Float3, "a_position" },
		{ buff_utils::ShaderDataType::Float4, "a_color" },
		{ buff_utils::ShaderDataType::Float, "a_radius" },
		{ buff_utils::ShaderDataType::Float, "a_thikness" },
		{ buff_utils::ShaderDataType::Float2, "a_localPos" },
		{ buff_utils::ShaderDataType::Int, "a_entityId" }
	};
	BufferLayout layout = BufferLayout(std::move(layoutData));
	m_circleVertexBuffer->setLayout(layout);
	m_circleVertexArray->addVertexBuffer(m_circleVertexBuffer);

	m_circleVertexBase = new CircleVertex[C_MAX_VERTICES];

	//-- Indices & Index buffer
	u32* indices = generateIndices();

	s_ptr<IndexBuffer> indexBuffer = s_ptr<IndexBuffer>(IndexBuffer::create(indices, C_MAX_INDICES));
	m_circleVertexArray->setIndexBuffer(indexBuffer);
	delete[] indices;

	auto& shaderLib = Application::subsystems().st<ShaderLibrary>();
	m_circleShader = shaderLib.load("assets/shaders/Circle.glsl");
}

void Renderer2D::init()
{
	PROFILE_FUNC;

	Application::subsystems().st<RenderCommand>().init();

	prepareQuadsDrawer();
	prepareCircleDrawer();
}

void Renderer2D::shutdown()
{
	delete[] m_quadVertexBase;
	delete[] m_circleVertexBase;
}

void Renderer2D::beginScene(const EditorCamera& camera)
{
	PROFILE_FUNC;

	m_currentFrameViewProjection = camera.VPMatrix();

	m_quadVertexPtr = m_quadVertexBase;
	m_circleVertexPtr = m_circleVertexBase;
	m_currQuadBatchIndex = 0;
	m_currCircleBatchIndex = 0;
	m_currTextureSlot = 1;
}

void Renderer2D::beginScene(const CameraComponent& camera, const glm::mat4& transform)
{
	PROFILE_FUNC;

	m_currentFrameViewProjection = camera.projection() * glm::inverse(transform);

	m_quadVertexPtr = m_quadVertexBase;
	m_circleVertexPtr = m_circleVertexBase;
	m_currQuadBatchIndex = 0;
	m_currCircleBatchIndex = 0;

	m_currTextureSlot = 1;
}

void Renderer2D::endScene()
{
	PROFILE_FUNC;

	flushQuads();
	flushCircles();
}

void Renderer2D::flushQuads()
{
	PROFILE_FUNC;

	m_quadVertexArray->bind();
	m_quadShader->bind();
	m_quadShader->uploadUniformMat4(m_currentFrameViewProjection, "u_vpMatrix");

	u32 dataSize = static_cast<u32>(
			reinterpret_cast<u8*>(m_quadVertexPtr) - reinterpret_cast<u8*>(m_quadVertexBase)
		);
	m_quadVertexBuffer->setData(m_quadVertexBase, dataSize);

	if (m_currQuadBatchIndex != 0)
	{
		for (u32 i = 0; i < m_currTextureSlot; ++i)
		{
			m_textureSlots[i]->bind(i);
		}
		Application::subsystems().st<RenderCommand>().drawIndexed(m_quadVertexArray, m_currQuadBatchIndex);
		m_frameStat.m_drawCalls++;
	}
}

void Renderer2D::flushCircles()
{
	m_circleVertexArray->bind();
	m_circleShader->bind();
	m_circleShader->uploadUniformMat4(m_currentFrameViewProjection, "u_vpMatrix");

	u32 dataSize = static_cast<u32>(
			reinterpret_cast<u8*>(m_circleVertexPtr) - reinterpret_cast<u8*>(m_circleVertexBase)
		);
	m_circleVertexBuffer->setData(m_circleVertexBase, dataSize);

	if (m_currCircleBatchIndex != 0)
	{
		Application::subsystems().st<RenderCommand>().drawIndexed(m_circleVertexArray, m_currCircleBatchIndex);
		m_frameStat.m_drawCalls++;
	}
}

void Renderer2D::drawQuad(const QuadCfg& cfg)
{
	PROFILE_FUNC;

	if (m_currQuadBatchIndex >= C_MAX_INDICES || m_currTextureSlot >= C_MAX_TEXTURE_SLOTS)
	{
		startNextBatch();
	}

	s_ptr<Texture2D> textureToUse = nullptr;
	switch (cfg.m_textureOpt)
	{
		case TextureOpt::FlatColored:
			textureToUse = m_whiteTexture;
			break;
		case TextureOpt::Textured:
			textureToUse = cfg.m_texture;
			break;
		default:
			break;
	}
	JNY_ASSERT(textureToUse.get() != nullptr, "Set texture on the quad");

	u32 textureIndex = C_INVALID_INDEX;
	for (u32 i = 0; i < m_currTextureSlot; ++i)
	{
		if (m_textureSlots[i]->rendererId() == textureToUse->rendererId())
		{
			textureIndex = i;
			break;
		}
	}

	if (textureIndex == C_INVALID_INDEX)
	{
		textureIndex = m_currTextureSlot;
		m_textureSlots[m_currTextureSlot] = std::move(textureToUse);
		++m_currTextureSlot;
	}

	const float textureIndexCastedToFloat = static_cast<float>(textureIndex);

	u8 idx = 0;
	std::ranges::for_each(m_quadVertexPosition, [&](const auto& pos)
		{
			m_quadVertexPtr->m_position = cfg.m_transform * pos;
			m_quadVertexPtr->m_color = cfg.m_color;
			m_quadVertexPtr->m_textureCoordinate = cfg.m_texturesPos[idx];
			m_quadVertexPtr->m_textureIndex = textureIndexCastedToFloat;
			m_quadVertexPtr->m_tilingFactor = cfg.m_tilingFactor;
			m_quadVertexPtr->m_entityId = cfg.m_entityId;
			m_quadVertexPtr++;
			++idx;
		});

	m_currQuadBatchIndex += C_INDICES_IN_QUAD;

	m_frameStat.m_quadCount++;
}

void Renderer2D::drawCircle(const CircleCfg& cfg)
{
	PROFILE_FUNC;

	if (m_currCircleBatchIndex >= C_MAX_INDICES)
	{
		startNextBatch();
	}

	u8 idx = 0;
	std::ranges::for_each(m_quadVertexPosition, [&](const auto& pos)
		{
			m_circleVertexPtr->m_color = cfg.m_color;
			m_circleVertexPtr->m_radius = cfg.m_radius;
			m_circleVertexPtr->m_thikness = cfg.m_thikness;
			m_circleVertexPtr->m_position = cfg.m_transform * pos;
			m_circleVertexPtr->m_localPos = pos * 2.0f;
			m_circleVertexPtr->m_entityId = cfg.m_entityId;
			m_circleVertexPtr++;
			++idx;
		});

	m_currCircleBatchIndex += C_INDICES_IN_QUAD;

	m_frameStat.m_quadCount++;
}

void Renderer2D::startNextBatch()
{
	endScene();

	m_quadVertexPtr = m_quadVertexBase;
	m_circleVertexPtr = m_circleVertexBase;
	m_currQuadBatchIndex = 0;
	m_currCircleBatchIndex = 0;
	m_currTextureSlot = 1;
}

void Renderer2D::windowResized(u32 width, u32 height)
{
	Application::subsystems().st<RenderCommand>().windowResized(width, height);
}

} //-- jny