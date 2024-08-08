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

GLenum toOpenGLType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Bool:		return GL_BOOL;
	case ShaderDataType::Int:		return GL_INT;
	case ShaderDataType::Int2:		return GL_INT;
	case ShaderDataType::Int3:		return GL_INT;
	case ShaderDataType::Int4:		return GL_INT;
	case ShaderDataType::Float:		return GL_FLOAT;
	case ShaderDataType::Float2:	return GL_FLOAT;
	case ShaderDataType::Float3:	return GL_FLOAT;
	case ShaderDataType::Float4:	return GL_FLOAT;
		//-- 3x3 matrix
	case ShaderDataType::Mat3:		return GL_FLOAT;
		//-- 4x4 matrix
	case ShaderDataType::Mat4:		return GL_FLOAT;
	default:
		break;
	}

	JNY_ASSERT(false);
	return 0;
}

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
	float vertices[3 * 7] =
	{
		-0.5f, -0.5f, 0.0f,0.2f, 0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 0.2f, 0.5f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.0f,  0.2f, 0.5f, 0.0f, 1.0f
	};

	//-- Vertex buffer
	m_vertexBuffer = std::unique_ptr<VertexBuffer>(VertexBuffer::create(vertices, 3 * 7));
	//-- Setting up vertext attribute array (layout for providing data splitting in shader)
	BufferLayout::LayoutData layoutData = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	BufferLayout layout = BufferLayout(std::move(layoutData));
	m_vertexBuffer->setLayout(layout);

	uint32_t layoutIndex = 0;
	for (const auto& element : layout)
	{
		//-- Enable 0 layout in our shader
		glEnableVertexAttribArray(layoutIndex);
		//-- Attribute pointer applied to EVERY VERTEX, not to all data
		//-- which means that:
		//-- index - index of layout we are setting up
		//-- size - amount of elements in that layout
		//-- type - data type for every element in that layout
		//-- stride - size in bytes for amount of elements in this layout (corresponding to vertex)
		//-- offset - how much bytes need to shift in vertexes to reach a start address of this layout from the start of the vertex
		glVertexAttribPointer(
			layoutIndex,
			element.m_count,
			toOpenGLType(element.m_type),
			element.m_normilized ? GL_TRUE : GL_FALSE,
			layout.stride(),
			reinterpret_cast<void*>(static_cast<uintptr_t>(element.m_offset))
		);

		++layoutIndex;
	}

	
	//-- Indicies
	uint32_t indecies[3] = { 0, 1, 2 };
	//-- Index buffer
	m_indexBuffer = std::unique_ptr<IndexBuffer>(IndexBuffer::create(indecies, 3));

	std::string vertexSrc =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec3 a_Position;\n"
		"layout(location = 1) in vec4 a_Color;\n"
		"out vec3 v_Position;\n"
		"out vec4 v_Color;\n"
		"\n"
		"void main()\n"
		"{\n"
			"gl_Position = vec4(a_Position, 1.0f);\n"
			"v_Position = a_Position;\n"
			"v_Color = a_Color;\n"
		"}\n";

	std::string fragmentSrc =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"in vec3 v_Position;\n"
		"in vec4 v_Color;\n"
		"\n"
		"void main()\n"
		"{\n"
			"color = v_Color;\n"
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