#include "jnypch.h"
#include "Renderer2D.h"
#include "Journey/Renderer/VertexArray.h"
#include "Journey/Renderer/Shader.h"
#include "Journey/Core/Application.h"

namespace jny
{

void Renderer2D::init()
{
	//-- Vertex array
	m_vertexArray = jny::Ref<jny::VertexArray>(jny::VertexArray::create());

	//-- Vertices
	float vertices[3 * 4] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	//-- Vertex buffer
	jny::Ref<jny::VertexBuffer> vertexBuffer = jny::Ref<jny::VertexBuffer>(jny::VertexBuffer::create(vertices, 3 * 4));
	//-- Setting up vertex attribute array (layout for providing data splitting in shader)
	jny::BufferLayout::LayoutData layoutData = {
		{ jny::ShaderDataType::Float3, "a_Position" },
	};
	jny::BufferLayout layout = jny::BufferLayout(std::move(layoutData));
	vertexBuffer->setLayout(layout);

	m_vertexArray->addVertexBuffer(vertexBuffer);

	//-- Indices
	uint32_t indecies[6] = { 0, 1, 2, 2, 3, 0 };
	//-- Index buffer
	jny::Ref<jny::IndexBuffer> indexBuffer = jny::Ref<jny::IndexBuffer>(jny::IndexBuffer::create(indecies, 6));

	m_vertexArray->setIndexBuffer(indexBuffer);

	auto& shaderLib = jny::Application::subsystems().st<jny::ShaderLibrary>();

	m_shader = shaderLib.load("resources/assets/shaders/FlatColor.glsl");
}

void Renderer2D::shutdown()
{

}

void Renderer2D::beginScene(const OrthographicCamera& camera)
{

}

void Renderer2D::endScene()
{

}

void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{

}

void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{

}

} //-- jny