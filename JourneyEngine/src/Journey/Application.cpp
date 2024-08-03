#include "jnypch.h"

#include "Journey/Application.h"
#include "Journey/Core.h"
#include "Journey/Log/Log.h"
#include "Journey/Events/ApplicationEvent.h"
#include "Journey/InputPoll.h"
#include "Journey/ImGui/ImGuiLayer.h"
#include "Journey/Window/Window.h"

#include <GLAD/glad.h>

namespace jny
{

std::unique_ptr<SingletonHolder> Application::s_sHolder;

Application::Application()
{
	JNY_ASSERT(s_instance == nullptr, "Can't have more than one instance of application")

	s_sHolder = std::make_unique<SingletonHolder>();

	s_sHolder->add<Window>(WindowData("Journey", 1200, 800));
	s_sHolder->add<InputPoll>();

	s_sHolder->st<Window>().setEventCallback([this](Event& _event)
		{
			onEvent(_event);
		});

	m_imGuiLayer = pushOverlay<ImGuiLayer>();

	//-- Vertex array
	glGenVertexArrays(1, &m_vertexArrayId);
	glBindVertexArray(m_vertexArrayId);
	//-- Vertex buffer
	glGenBuffers(1, &m_vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	//-- Vertices
	float vertices[3 * 3] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, vertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	//-- Attrib pointer applied to EVERY VERTEX, not to all data
	//-- which means that:
	//-- index - index of layout we are setting up
	//-- size - amount of elements in that layout
	//-- type - data type for every element in that layout
	//-- stride - size in bytes for amount of elements in this layout (corresponding to vertex)
	//-- offset - how much bytes need to shift in vertexes to reach a start addres of this layout from the start of the vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
	
	//-- Index buffer
	glGenBuffers(1, &m_indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	//-- Indecies
	uint32_t indecies[3] = { 0, 1, 2 };
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 3, indecies, GL_STATIC_DRAW);
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

		glBindVertexArray(m_vertexArrayId);
		//-- Elements is indexes!
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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
	dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& event)
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