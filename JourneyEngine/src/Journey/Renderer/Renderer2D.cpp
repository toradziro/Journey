#include "jnypch.h"
#include "Renderer2D.h"
#include "Journey/Renderer/VertexArray.h"
#include "Journey/Renderer/Shader.h"
#include "Journey/Renderer/RenderCommand.h"
#include "Journey/Core/Application.h"
#include "Journey/Core/Profiling/TimeInstruments.h"

#include <glm/gtc/matrix_transform.hpp>

namespace
{

constexpr	uint32_t C_MAX_VERTICES = jny::Renderer2D::C_MAX_QUADS_IN_A_BATCH * jny::Renderer2D::C_VERTICES_IN_QUAD;
constexpr	uint32_t C_MAX_INDICES = jny::Renderer2D::C_MAX_QUADS_IN_A_BATCH * jny::Renderer2D::C_INDICES_IN_QUAD;
constexpr	uint32_t C_INVALID_INDEX = std::numeric_limits<uint32_t>::max();

} //-- unnamed

namespace jny
{

void Renderer2D::init()
{
	PROFILE_FUNC;

	//-- Vertex array
	m_quadVertexArray = Ref<VertexArray>(VertexArray::create());

	//-- Vertex buffer
	m_quadVertexBuffer = Ref<VertexBuffer>(VertexBuffer::create(C_MAX_VERTICES * sizeof(QuadVertex)));
	//-- Setting up vertex attribute array (layout for providing data splitting in shader)
	BufferLayout::LayoutData layoutData = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" },
		{ ShaderDataType::Float2, "a_TexturePos" },
		{ ShaderDataType::Float, "a_TextureIndex" },
		{ ShaderDataType::Float, "a_TilingFactor" }
	};
	BufferLayout layout = BufferLayout(std::move(layoutData));
	m_quadVertexBuffer->setLayout(layout);
	m_quadVertexArray->addVertexBuffer(m_quadVertexBuffer);

	m_quadVertexBase = new QuadVertex[C_MAX_VERTICES];

	//-- Indices & Index buffer
	uint32_t* indices = new uint32_t[C_MAX_INDICES];
	
	uint32_t offset = 0;
	for (uint32_t i = 0; i < C_MAX_INDICES; i += C_INDICES_IN_QUAD)
	{
		indices[i] = offset;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;

		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset;

		offset += C_VERTICES_IN_QUAD;
	}

	Ref<IndexBuffer> indexBuffer = Ref<IndexBuffer>(IndexBuffer::create(indices, C_MAX_INDICES));
	m_quadVertexArray->setIndexBuffer(indexBuffer);
	delete[] indices;

	auto& shaderLib = Application::subsystems().st<ShaderLibrary>();
	m_textureShader = shaderLib.load("assets/shaders/Texture.glsl");
	m_textureShader->bind();

	m_whiteTexture = Texture2D::create(1, 1);
	uint32_t whiteTextureData = 0xffffffff;
	m_whiteTexture->setData(static_cast<void*>(&whiteTextureData), sizeof(whiteTextureData));
	m_textureSlots[0] = m_whiteTexture;

	int32_t samplers[C_MAX_TEXTURE_SLOTS]{};
	for (uint32_t i = 0; i < C_MAX_TEXTURE_SLOTS; ++i)
	{
		samplers[i] = i;
	}
	m_textureShader->uploadUniformIntArray(samplers, C_MAX_TEXTURE_SLOTS, "u_textures");

	m_quadVertexPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	m_quadVertexPosition[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	m_quadVertexPosition[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
	m_quadVertexPosition[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
}

void Renderer2D::shutdown()
{
	delete[] m_quadVertexBase;
}

void Renderer2D::beginScene(const OrthographicCamera& camera)
{
	PROFILE_FUNC;

	m_quadVertexArray->bind();

	m_textureShader->bind();
	m_textureShader->uploadUniformMat4(camera.viewProjectionMatrix(), "u_vpMatrix");

	m_quadVertexPtr = m_quadVertexBase;
	m_currQuadIndex = 0;
	m_currTextureSlot = 1;
	m_currCamera = &camera;
}

void Renderer2D::endScene()
{
	PROFILE_FUNC;

	uint32_t dataSize = static_cast<uint32_t>(
		reinterpret_cast<uint8_t*>(m_quadVertexPtr) - reinterpret_cast<uint8_t*>(m_quadVertexBase)
	);
	m_quadVertexBuffer->setData(m_quadVertexBase, dataSize);

	flush();
}

void Renderer2D::flush()
{
	PROFILE_FUNC;

	for (uint32_t i = 0; i < m_currTextureSlot; ++i)
	{
		m_textureSlots[i]->bind(i);
	}

	Application::subsystems().st<RenderCommand>().drawIndexed(m_quadVertexArray, m_currQuadIndex);
	m_frameStat.m_drawCalls++;
}

void Renderer2D::drawQuad(const QuadCfg& cfg)
{
	PROFILE_FUNC;

	if (m_currQuadIndex >= C_MAX_INDICES || m_currTextureSlot >= C_MAX_TEXTURE_SLOTS)
	{
		startNextBatch();
	}

	Ref<Texture2D> textureToUse = nullptr;
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
	JNY_ASSERT(textureToUse.raw() != nullptr, "Set texture on the quad");

	uint32_t textureIndex = C_INVALID_INDEX;
	for (uint32_t i = 0; i < m_currTextureSlot; ++i)
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
		m_textureSlots[textureIndex] = textureToUse;
		++m_currTextureSlot;
	}

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, cfg.m_position);
	//-- Rotation is hard operation, so don't use it unless it's really necessary
	if (cfg.m_rotateOpt == RotateOpt::Rotated)
	{
		transform = glm::rotate(transform, cfg.m_rotation, { 0.0f, 0.0f, 1.0f });
	}
	transform = glm::scale(transform, { cfg.m_size.x, cfg.m_size.y, 0.0f });

	const float textureIndexCastedToFloat = static_cast<float>(textureIndex);

	uint8_t idx = 0;
	std::ranges::for_each(m_quadVertexPosition, [&](const auto& pos)
		{
			m_quadVertexPtr->m_position = transform * pos;
			m_quadVertexPtr->m_color = cfg.m_color;
			m_quadVertexPtr->m_textureCoordinate = cfg.m_texturesPos[idx];
			m_quadVertexPtr->m_textureIndex = textureIndexCastedToFloat;
			m_quadVertexPtr->m_tilingFactor = cfg.m_tilingFactor;
			m_quadVertexPtr++;
			++idx;
		});

	m_currQuadIndex += C_INDICES_IN_QUAD;

	m_frameStat.m_quadCount++;
}

void Renderer2D::startNextBatch()
{
	endScene();

	m_quadVertexPtr = m_quadVertexBase;
	m_currQuadIndex = 0;
	m_currTextureSlot = 1;
}

} //-- jny