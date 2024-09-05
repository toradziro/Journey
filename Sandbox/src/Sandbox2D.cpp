#include "jnypch.h"
#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() :
	Layer("Sandbox2Ds"),
	m_orthoCameraCtrl(jny::Application::aspectRatio())
{ }

void Sandbox2D::attach()
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

void Sandbox2D::detach()
{
}

void Sandbox2D::update(float dt)
{
	m_orthoCameraCtrl.update(dt);

	auto& renderer = jny::Application::subsystems().st<jny::Renderer>();

	renderer.setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	renderer.clear();

	//-- Start rendering
	renderer.beginScene(m_orthoCameraCtrl.camera());

	m_shader->bind();
	m_shader->uploadUniformFloat4(m_squareColor, "u_color");

	//-- Submit data we want to render, if we wanna submit more VA's - we use more submission calls
	renderer.submit(m_vertexArray, m_shader);

	//-- End rendering
	renderer.endScene();
}

void Sandbox2D::onEvent(jny::Event& event)
{
	m_orthoCameraCtrl.onEvent(event);
}

void Sandbox2D::imGuiRender()
{
	ImGui::Begin("Color prop");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_squareColor));
	ImGui::End();
}
