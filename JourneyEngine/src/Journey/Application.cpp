#include "jnypch.h"

#include "Journey/Application.h"
#include "Journey/Core.h"
#include "Journey/Log/Log.h"
#include "Journey/Events/ApplicationEvent.h"
#include "Journey/InputPoll.h"
#include "Journey/ImGui/ImGuiLayer.h"
#include "Journey/Window/Window.h"
#include "Journey/Renderer/Buffer.h"
#include "Journey/Renderer/Renderer.h"

#include <GLAD/glad.h>

namespace jny
{

std::unique_ptr<SingletonHolder> Application::s_sHolder;

Application::Application()
{
	s_sHolder = std::make_unique<SingletonHolder>();

	s_sHolder->add<Window>(WindowData("Journey", 1200, 800));
	s_sHolder->add<InputPoll>();
	s_sHolder->add<Renderer>(RendererAPI::OpenGL);

	s_sHolder->st<Window>().setEventCallback([this](Event& _event)
		{
			onEvent(_event);
		});

	m_imGuiLayer = pushOverlay<ImGuiLayer>();

	//-- Vertex array
	glGenVertexArrays(1, &m_vertexArrayId);
	glBindVertexArray(m_vertexArrayId);

	//-- Vertices
	float vertices[3 * 3] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
	};

	//-- Vertex buffer
	m_vertexBuffer = std::unique_ptr<VertexBuffer>(VertexBuffer::create(vertices, 9));

	glEnableVertexAttribArray(0);
	//-- Attribute pointer applied to EVERY VERTEX, not to all data
	//-- which means that:
	//-- index - index of layout we are setting up
	//-- size - amount of elements in that layout
	//-- type - data type for every element in that layout
	//-- stride - size in bytes for amount of elements in this layout (corresponding to vertex)
	//-- offset - how much bytes need to shift in vertexes to reach a start address of this layout from the start of the vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	
	//-- Indicies
	uint32_t indecies[3] = { 0, 1, 2 };
	//-- Index buffer
	m_indexBuffer = std::unique_ptr<IndexBuffer>(IndexBuffer::create(indecies, 3));

	std::string vertexSrc =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec3 a_Position;\n"
		"out vec3 v_Position;"
		"\n"
		"void main()\n"
		"{\n"
			"gl_Position = vec4(a_Position, 1.0f);\n"
			"v_Position = a_Position;\n"
		"}\n";

	std::string fragmentSrc =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"in vec3 v_Position;"
		"\n"
		"void main()\n"
		"{\n"
			"color = vec4(v_Position * 0.5f + 0.5f, 1.0f);\n"
		"}\n";

	m_shader = std::unique_ptr<Shader>(Shader::create(std::move(vertexSrc), std::move(fragmentSrc)));
}

Application::~Application()
{
	s_sHolder.reset();
}

void Application::run()
{
	while (m_running)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_shader->bind();
		glBindVertexArray(m_vertexArrayId);
		//-- Elements is indexes!
		glDrawElements(GL_TRIANGLES, m_indexBuffer->count(), GL_UNSIGNED_INT, nullptr);

		for (auto& layer : m_layers)
		{
			layer->update();
		}

		m_imGuiLayer->begin();
		for (auto& layer : m_layers)
		{
			layer->imGuiRender();
		}
		m_imGuiLayer->end();

		s_sHolder->st<Window>().update();
	}
}

void Application::onEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent&)
		{
			return windowCloseEvent();
		});

	for (auto& layer : m_layers)
	{
		layer->onEvent(event);
		if (event.handeled())
		{
			break;
		}
	}
}

bool Application::windowCloseEvent()
{
	m_running = false;
	return true;
}

void Application::popLayer(Layer* layer)
{
	layer->detach();
	m_layers.popLayer(layer);
}

void Application::popOverlay(Layer* layer)
{
	layer->detach();
	m_layers.popOverlay(layer);
}

} //-- jny