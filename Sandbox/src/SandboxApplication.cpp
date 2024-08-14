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
		updateCamera();

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

	void onEvent(jny::Event& /*event*/) override
	{
		//jny::EventDispatcher dispatcher(event);
	}

	void imGuiRender() override
	{
		if (m_scenePropsWindowOpen)
		{
			ImGui::Begin("Scene properties", &m_scenePropsWindowOpen);
			
			constexpr auto tableFlags = ImGuiTableFlags_NoSavedSettings
				| ImGuiTableFlags_NoBordersInBodyUntilResize
				| ImGuiTableFlags_Resizable;

			if (ImGui::BeginTable("##scenePropsTable", 2, tableFlags, ImGui::GetContentRegionAvail()))
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Camera speed");

				ImGui::TableNextColumn();
				ImGui::DragFloat("##cameraSpeedScalar", &m_cameraSpeed, 0.01f, 0.0f, 1.0f);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Rotation speed");

				ImGui::TableNextColumn();
				ImGui::DragFloat("##cameraRotationSpeedScalar", &m_cameraRotationSpeed, 1.0f, 0.0f, 20.0f);

				ImGui::EndTable();
			}
			ImGui::End();
		}
	}

	void updateCamera()
	{
		auto& inputPollSystem = jny::Application::subsystems().st<jny::InputPoll>();
		if (inputPollSystem.keyPressed(GLFW_KEY_RIGHT))
		{
			m_cameraPos.x -= m_cameraSpeed;
		}
		else if (inputPollSystem.keyPressed(GLFW_KEY_LEFT))
		{
			m_cameraPos.x += m_cameraSpeed;
		}

		if (inputPollSystem.keyPressed(GLFW_KEY_UP))
		{
			m_cameraPos.y += m_cameraSpeed;
		}
		else if (inputPollSystem.keyPressed(GLFW_KEY_DOWN))
		{
			m_cameraPos.y -= m_cameraSpeed;
		}

		if (inputPollSystem.keyPressed(GLFW_KEY_Q))
		{
			m_cameraRotation += m_cameraRotationSpeed;
		}
		else if (inputPollSystem.keyPressed(GLFW_KEY_E))
		{
			m_cameraRotation -= m_cameraRotationSpeed;
		}
		m_orthoCamera->setPosition(m_cameraPos);
		m_orthoCamera->setRotation(m_cameraRotation);
	}

private:
	std::shared_ptr<jny::Shader>				m_shader;
	std::shared_ptr<jny::VertexArray>			m_vertexArray;
	std::shared_ptr<jny::OrthographicCamera>	m_orthoCamera;

	glm::vec3									m_cameraPos = { 0.0f, 0.0f, 0.0f };
	float										m_cameraRotation = 0.0f;

	float										m_cameraSpeed = 0.01f;
	float										m_cameraRotationSpeed = 1.0f;
	bool										m_scenePropsWindowOpen = true;
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
