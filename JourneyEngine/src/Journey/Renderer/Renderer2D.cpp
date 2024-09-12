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

void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	drawQuad({position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
	PROFILE_FUNC;

	//-- must be bound while sending color, uncomment if scenarios changes a shader
	//m_quadVertexArray->bind();
	//m_textureShader->bind();
	m_textureShader->uploadUniformFloat4(color, "u_color");

	m_whiteTexture->bind();

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
	transform = glm::scale(transform, { size.x, size.y, 0.0f });
	m_textureShader->uploadUniformMat4(transform, "u_modelTransform");

	Application::subsystems().st<RenderCommand>().drawIndexed(m_quadVertexArray);
}

void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
{
	drawQuad({ position.x, position.y, 0.0f }, size, texture);
}

void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
{
	PROFILE_FUNC;

	//-- must be bound while sending color, uncomment if scenarios changes a shader
	//m_quadVertexArray->bind();
	//m_textureShader->bind();
	m_textureShader->uploadUniformFloat4(glm::vec4(1.0f), "u_color");

	texture->bind();

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
	transform = glm::scale(transform, { size.x, size.y, 0.0f });
	m_textureShader->uploadUniformMat4(transform, "u_modelTransform");

	Application::subsystems().st<RenderCommand>().drawIndexed(m_quadVertexArray);
}

} //-- jny