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

constexpr	uint32_t C_VERTICES_IN_QUAD = 4;
constexpr	uint32_t C_INDICES_IN_QUAD = 6;
constexpr	uint32_t C_MAX_QUADS_IN_A_BATCH = 10000;
constexpr	uint32_t C_MAX_VERTICES = C_MAX_QUADS_IN_A_BATCH * C_VERTICES_IN_QUAD;
constexpr	uint32_t C_MAX_INDICES = C_MAX_QUADS_IN_A_BATCH * C_INDICES_IN_QUAD;

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
		{ ShaderDataType::Float2, "a_TexturePos" }
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
	m_textureShader = shaderLib.load("resources/assets/shaders/Texture.glsl");
	m_textureShader->bind();
	//m_textureShader->uploadUniformInt(0, "u_texture");

	m_whiteTexture = Texture2D::create(1, 1);
	uint32_t whiteTextureData = 0xffffffff;
	m_whiteTexture->setData(static_cast<void*>(&whiteTextureData), sizeof(whiteTextureData));
}

void Renderer2D::shutdown() { }

void Renderer2D::beginScene(const OrthographicCamera& camera)
{
	PROFILE_FUNC;

	m_quadVertexArray->bind();

	m_textureShader->bind();
	m_textureShader->uploadUniformMat4(camera.viewProjectionMatrix(), "u_vpMatrix");

	m_quadVertexPtr = m_quadVertexBase;
	m_currQuadIndex = 0;
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

	Application::subsystems().st<RenderCommand>().drawIndexed(m_quadVertexArray, m_currQuadIndex);
}

void Renderer2D::drawQuad(const QuadCfg& cfg)
{
	PROFILE_FUNC;

	m_quadVertexPtr->m_position = cfg.m_position;
	m_quadVertexPtr->m_color = cfg.m_color;
	m_quadVertexPtr->m_textureCoordinate = { 0.0f, 0.0f };
	m_quadVertexPtr++;

	m_quadVertexPtr->m_position = { cfg.m_position.x + cfg.m_size.x, cfg.m_position.y, cfg.m_position.z };
	m_quadVertexPtr->m_color = cfg.m_color;
	m_quadVertexPtr->m_textureCoordinate = { 1.0f, 0.0f };
	m_quadVertexPtr++;

	m_quadVertexPtr->m_position = { cfg.m_position.x + cfg.m_size.x, cfg.m_position.y + cfg.m_size.y, cfg.m_position.z };
	m_quadVertexPtr->m_color = cfg.m_color;
	m_quadVertexPtr->m_textureCoordinate = { 1.0f, 1.0f };
	m_quadVertexPtr++;

	m_quadVertexPtr->m_position = { cfg.m_position.x, cfg.m_position.y + cfg.m_size.y, cfg.m_position.z };
	m_quadVertexPtr->m_color = cfg.m_color;
	m_quadVertexPtr->m_textureCoordinate = { 0.0f, 1.0f };
	m_quadVertexPtr++;

	m_currQuadIndex += C_INDICES_IN_QUAD;

	//Ref<Texture2D> textureToUse = nullptr;

	//switch (cfg.m_textureOpt)
	//{
	//case TextureOpt::FlatColored:
	//	textureToUse = m_whiteTexture;
	//	break;
	//case TextureOpt::Textured:
	//	textureToUse = cfg.m_texture;
	//	break;
	//default:
	//	break;
	//}
	//JNY_ASSERT(textureToUse.raw() != nullptr, "Set texture on the quad");

	//textureToUse->bind();

	//glm::mat4 transform = glm::mat4(1.0f);
	//transform = glm::translate(transform, cfg.m_position);
	//transform = glm::scale(transform, { cfg.m_size.x, cfg.m_size.y, 0.0f });
	////-- Rotation is hard operation, so don't use it unless it's really necessary
	//if (cfg.m_rotateOpt == RotateOpt::Rotated)
	//{
	//	transform = glm::rotate(transform, cfg.m_rotation, { 0.0f, 0.0f, 1.0f });
	//}
	//m_quadVertexPtr->m_position *= transform;
	////m_textureShader->uploadUniformMat4(transform, "u_modelTransform");

	//Application::subsystems().st<RenderCommand>().drawIndexed(m_quadVertexArray);
}

} //-- jny