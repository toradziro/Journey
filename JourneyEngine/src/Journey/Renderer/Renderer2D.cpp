#include "jnypch.h"
#include "Renderer2D.h"
#include "Journey/Renderer/VertexArray.h"
#include "Journey/Renderer/Shader.h"
#include "Journey/Renderer/RenderCommand.h"
#include "Journey/Core/Application.h"
#include "Journey/Core/Profiling/TimeInstruments.h"

#include <glm/gtc/matrix_transform.hpp>

namespace jny
{

void Renderer2D::init()
{
	PROFILE_FUNC;

	//-- Vertex array
	m_quadVertexArray = Ref<VertexArray>(VertexArray::create());

	//-- Vertices
	float vertices[5 * 4] =
	{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
	};

	//-- Vertex buffer
	Ref<VertexBuffer> vertexBuffer = Ref<VertexBuffer>(VertexBuffer::create(vertices, 5 * 4));
	//-- Setting up vertex attribute array (layout for providing data splitting in shader)
	BufferLayout::LayoutData layoutData = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float2, "a_TexturePos" }
	};
	BufferLayout layout = BufferLayout(std::move(layoutData));
	vertexBuffer->setLayout(layout);
	m_quadVertexArray->addVertexBuffer(vertexBuffer);

	//-- Indices
	uint32_t indecies[6] = { 0, 1, 2, 2, 3, 0 };
	//-- Index buffer
	Ref<IndexBuffer> indexBuffer = Ref<IndexBuffer>(IndexBuffer::create(indecies, 6));
	m_quadVertexArray->setIndexBuffer(indexBuffer);

	auto& shaderLib = Application::subsystems().st<ShaderLibrary>();
	m_textureShader = shaderLib.load("resources/assets/shaders/Texture.glsl");
	m_textureShader->bind();
	m_textureShader->uploadUniformInt(0, "u_texture");

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
}

void Renderer2D::endScene() { }

void Renderer2D::drawQuad(const QuadCfg& cfg)
{
	PROFILE_FUNC;

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

	//-- must be bound while sending color, uncomment if scenarios changes a shader
	m_textureShader->uploadUniformFloat4(cfg.m_color, "u_color");
	m_textureShader->uploadUniformFloat(cfg.m_tilingFactor, "u_tilingFactor");

	textureToUse->bind();

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, cfg.m_position);
	transform = glm::scale(transform, { cfg.m_size.x, cfg.m_size.y, 0.0f });
	//-- Rotation is hard operation, so don't use it unless it's really necessary
	if (cfg.m_rotateOpt == RotateOpt::Rotated)
	{
		transform = glm::rotate(transform, cfg.m_rotation, { 0.0f, 0.0f, 1.0f });
	}
	m_textureShader->uploadUniformMat4(transform, "u_modelTransform");

	Application::subsystems().st<RenderCommand>().drawIndexed(m_quadVertexArray);
}

} //-- jny