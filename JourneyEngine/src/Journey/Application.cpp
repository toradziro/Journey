#include "jnypch.h"

#include "Journey/Application.h"
#include "Journey/Core.h"
#include "Journey/Log/Log.h"
#include "Journey/Events/ApplicationEvent.h"
#include "Journey/InputPoll.h"
#include "Journey/ImGui/ImGuiLayer.h"
#include "Journey/Window/Window.h"
#include "Journey/Renderer/Buffer.h"
#include "Journey/Renderer/VertexArray.h"
#include "Journey/Renderer/Shader.h"
#include "Journey/Renderer/Renderer.h"
#include "Journey/Renderer/RenderCommand.h"
#include "Journey/Renderer/OrthographicCamera.h"

namespace jny
{

std::unique_ptr<SingletonHolder> Application::s_sHolder;

Application::Application()
	: m_orthoCamera(new OrthographicCamera(-1.0f, 1.0f, -1.0f, 1.0f))
{
	s_sHolder = std::make_unique<SingletonHolder>();

	//-- Create main window
	s_sHolder->add<Window>(WindowData("Journey", 1200, 800));
	//-- Easy way to check if any events ongoing
	s_sHolder->add<InputPoll>();
	//-- Creating renderer based on API we want to use for rendering
	//-- RenderCommnad is a holder for API implementation and also caller for rendering methods
	s_sHolder->add<RenderCommand>();
	s_sHolder->st<RenderCommand>().createRenderer(RendererAPI::API::OpenGL);
	//-- Interface for rendering
	s_sHolder->add<Renderer>();

	//-- Prepare to process events
	s_sHolder->st<Window>().setEventCallback([this](Event& _event)
		{
			onEvent(_event);
		});

	m_imGuiLayer = pushOverlay<ImGuiLayer>();

	//-- Vertex array
	m_vertexArray = std::shared_ptr<VertexArray>(VertexArray::create());

	//-- Vertices
	float vertices[3 * 7] =
	{
		-0.5f, -0.5f, 0.0f,0.2f, 0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 0.2f, 0.0f, 0.5f, 1.0f,
		0.0f, 0.5f, 0.0f,  0.9f, 0.2f, 0.0f, 1.0f
	};

	//-- Vertex buffer
	std::shared_ptr<VertexBuffer> vertexBuffer = std::shared_ptr<VertexBuffer>(VertexBuffer::create(vertices, 3 * 7));
	//-- Setting up vertex attribute array (layout for providing data splitting in shader)
	BufferLayout::LayoutData layoutData = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	BufferLayout layout = BufferLayout(std::move(layoutData));
	vertexBuffer->setLayout(layout);

	m_vertexArray->addVertexBuffer(vertexBuffer);
	
	//-- Indices
	uint32_t indecies[3] = { 0, 1, 2 };
	//-- Index buffer
	std::shared_ptr<IndexBuffer> indexBuffer = std::shared_ptr<IndexBuffer>(IndexBuffer::create(indecies, 3));

	std::string vertexSrc =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec3 a_Position;\n"
		"layout(location = 1) in vec4 a_Color;\n"
		"uniform mat4 u_vpMatrix;\n"
		"out vec3 v_Position;\n"
		"out vec4 v_Color;\n"
		"\n"
		"void main()\n"
		"{\n"
			"gl_Position = u_vpMatrix * vec4(a_Position, 1.0f);\n"
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

	m_vertexArray->setIndexBuffer(indexBuffer);

	m_shader = std::shared_ptr<Shader>(Shader::create(std::move(vertexSrc), std::move(fragmentSrc)));
}

Application::~Application()
{
	s_sHolder.reset();
}

void Application::run()
{
	while (m_running)
	{
		auto& renderer = Application::subsystems().st<Renderer>();

		renderer.setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		renderer.clear();

		m_orthoCamera->setPosition({0.2f, 0.2f, 0.0f});
		m_orthoCamera->setRotation(45.0f);

		//-- Start rendering
		renderer.beginScene(m_orthoCamera);
		
		//-- Submit data we want to render, if we wanna submit more VA's - we use more submission calls
		renderer.submit(m_vertexArray, m_shader);
		
		//-- End rendering
		renderer.endScene();

		for (auto& layer : m_layers)
		{
			layer->update();
		}
		
		//-- ImGui drawing
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

	for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
	{
		(*it)->onEvent(event);
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