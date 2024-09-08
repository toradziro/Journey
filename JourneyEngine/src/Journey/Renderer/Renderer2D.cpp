#include "jnypch.h"
#include "Renderer2D.h"
#include "Journey/Renderer/VertexArray.h"
#include "Journey/Renderer/Shader.h"
#include "Journey/Renderer/RenderCommand.h"
#include "Journey/Core/Application.h"

namespace jny
{

void Renderer2D::init()
{
	//-- Vertex array
	m_quadVertexArray = Ref<VertexArray>(VertexArray::create());

	//-- Vertices
	float vertices[3 * 4] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	//-- Vertex buffer
	Ref<VertexBuffer> vertexBuffer = Ref<VertexBuffer>(VertexBuffer::create(vertices, 3 * 4));
	//-- Setting up vertex attribute array (layout for providing data splitting in shader)
	BufferLayout::LayoutData layoutData = {
		{ ShaderDataType::Float3, "a_Position" },
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
	m_flatColorShader = shaderLib.load("resources/assets/shaders/FlatColor.glsl");
}

void Renderer2D::shutdown() { }

void Renderer2D::beginScene(const OrthographicCamera& camera)
{
	m_flatColorShader->bind();
	m_quadVertexArray->bind();

	m_flatColorShader->uploadUniformMat4(camera.viewProjectionMatrix(), "u_vpMatrix");
	m_flatColorShader->uploadUniformMat4(glm::mat4(1.0f), "u_modelTransform");
}

void Renderer2D::endScene()
{

}

void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	//-- must be bound while sending color, uncomment if scenarios changes a shader
	//m_flatColorShader->bind();
	//m_quadVertexArray->bind();

	drawQuad({position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
	//-- must be bound while sending color, uncomment if scenarios changes a shader
	//m_flatColorShader->bind();
	//m_quadVertexArray->bind();

	m_flatColorShader->uploadUniformFloat4(color, "u_color");
	Application::subsystems().st<RenderCommand>().drawIndexed(m_quadVertexArray);
}

} //-- jny