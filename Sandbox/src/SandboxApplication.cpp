#include "common.h"

#include "SandboxApplication.h"
#include "Journey/InputPoll.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class ExampleLayer : public jny::Layer
{
public:

	ExampleLayer() : Layer("Test")
	{
		m_orthoCamera = std::shared_ptr<jny::OrthographicCamera>(new jny::OrthographicCamera(-1.0f, 1.0f, -1.0f, 1.0f));

		//-- Vertex array
		m_vertexArray = std::shared_ptr<jny::VertexArray>(jny::VertexArray::create());

		//-- Vertices
		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f,0.2f, 0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.0f, 0.5f, 1.0f,
			0.0f, 0.5f, 0.0f,  0.9f, 0.2f, 0.0f, 1.0f
		};

		//-- Vertex buffer
		std::shared_ptr<jny::VertexBuffer> vertexBuffer = std::shared_ptr<jny::VertexBuffer>(jny::VertexBuffer::create(vertices, 3 * 7));
		//-- Setting up vertex attribute array (layout for providing data splitting in shader)
		jny::BufferLayout::LayoutData layoutData = {
			{ jny::ShaderDataType::Float3, "a_Position" },
			{ jny::ShaderDataType::Float4, "a_Color" }
		};
		jny::BufferLayout layout = jny::BufferLayout(std::move(layoutData));
		vertexBuffer->setLayout(layout);

		m_vertexArray->addVertexBuffer(vertexBuffer);

		//-- Indices
		uint32_t indecies[3] = { 0, 1, 2 };
		//-- Index buffer
		std::shared_ptr<jny::IndexBuffer> indexBuffer = std::shared_ptr<jny::IndexBuffer>(jny::IndexBuffer::create(indecies, 3));

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

		m_shader = std::shared_ptr<jny::Shader>(jny::Shader::create(std::move(vertexSrc), std::move(fragmentSrc)));
	}

	void update() override
	{
		auto& renderer = jny::Application::subsystems().st<jny::Renderer>();

		renderer.setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		renderer.clear();

		//-- Start rendering
		renderer.beginScene(m_orthoCamera);

		//-- Submit data we want to render, if we wanna submit more VA's - we use more submission calls
		renderer.submit(m_vertexArray, m_shader);

		//-- End rendering
		renderer.endScene();
	}

	void onEvent(jny::Event& event) override
	{
		jny::EventDispatcher dispatcher(event);
		dispatcher.dispatch<jny::KeyPressedEvent>([&](jny::KeyPressedEvent& event)
			{
				constexpr float C_SPEED = 0.02f;
				auto currPos = m_orthoCamera->position();
				//-- Camera is ortho so we won't see any difference with z component right now
				if (event.keyCode() == GLFW_KEY_W)
				{
					currPos.z += C_SPEED;
					m_orthoCamera->setPosition(currPos);
					return true;
				}
				else if (event.keyCode() == GLFW_KEY_S)
				{
					currPos.z -= C_SPEED;
					m_orthoCamera->setPosition(currPos);
					return true;
				}
				else if (event.keyCode() == GLFW_KEY_A)
				{
					currPos.x -= C_SPEED;
					m_orthoCamera->setPosition(currPos);
					return true;
				}
				else if (event.keyCode() == GLFW_KEY_D)
				{
					currPos.x += C_SPEED;
					m_orthoCamera->setPosition(currPos);
					return true;
				}
				else if (event.keyCode() == GLFW_KEY_Q)
				{
					currPos.y += C_SPEED;
					m_orthoCamera->setPosition(currPos);
					return true;
				}
				else if (event.keyCode() == GLFW_KEY_E)
				{
					currPos.y -= C_SPEED;
					m_orthoCamera->setPosition(currPos);
					return true;
				}
				return false;
			});
	}

	void imGuiRender() override
	{
	}

private:
	std::shared_ptr<jny::Shader>				m_shader;
	std::shared_ptr<jny::VertexArray>			m_vertexArray;
	std::shared_ptr<jny::OrthographicCamera>	m_orthoCamera;
};

Sandbox::Sandbox()
{
	this->pushLayer<ExampleLayer>();
}

Sandbox::~Sandbox()
{

}

jny::Application* jny::createApplication()
{
	return new Sandbox();
}
