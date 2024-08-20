#include "common.h"

#include "SandboxApplication.h"
#include "Journey/InputPoll.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class ExampleLayer : public jny::Layer
{
public:

	ExampleLayer() : Layer("Test")
	{
		m_orthoCamera = jny::Ref<jny::OrthographicCamera>(new jny::OrthographicCamera(-1.0f, 1.0f, -1.0f, 1.0f));

		//-- Vertex array
		m_vertexArray = jny::Ref<jny::VertexArray>(jny::VertexArray::create());

		//-- Vertices
		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
		};

		//-- Vertex buffer
		jny::Ref<jny::VertexBuffer> vertexBuffer = jny::Ref<jny::VertexBuffer>(jny::VertexBuffer::create(vertices, 3 * 7));
		//-- Setting up vertex attribute array (layout for providing data splitting in shader)
		jny::BufferLayout::LayoutData layoutData = {
			{ jny::ShaderDataType::Float3, "a_Position" }
		};
		jny::BufferLayout layout = jny::BufferLayout(std::move(layoutData));
		vertexBuffer->setLayout(layout);

		m_vertexArray->addVertexBuffer(vertexBuffer);

		//-- Indices
		uint32_t indecies[3] = { 0, 1, 2 };
		//-- Index buffer
		jny::Ref<jny::IndexBuffer> indexBuffer = jny::Ref<jny::IndexBuffer>(jny::IndexBuffer::create(indecies, 3));

		std::string vertexSrc =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) in vec3 a_Position;\n"
			"\n"
			"uniform mat4 u_vpMatrix;\n"
			"uniform mat4 u_modelTransform;\n"
			"\n"
			"out vec3 v_Position;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	gl_Position = u_vpMatrix * u_modelTransform * vec4(a_Position, 1.0f);\n"
			"	v_Position = a_Position;\n"
			"}\n";

		std::string fragmentSrc =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) out vec4 color;\n"
			"in vec3 v_Position;\n"
			"uniform vec3 u_color;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	color = vec4(u_color, 1.0f);\n"
			"}\n";

		m_vertexArray->setIndexBuffer(indexBuffer);

		m_shader = jny::Ref<jny::Shader>(jny::Shader::create(std::move(vertexSrc), std::move(fragmentSrc)));
	}

	void update(float dt) override
	{
		m_deltaTime = dt;

		updateCamera();

		auto& renderer = jny::Application::subsystems().st<jny::Renderer>();

		renderer.setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		renderer.clear();

		//-- Start rendering
		renderer.beginScene(m_orthoCamera);

		m_modelTransform = glm::mat4(1.0f);
		m_modelTransform = glm::translate(m_modelTransform, m_modelPosition);
		m_modelTransform = glm::scale(m_modelTransform, m_modelScale);
		
		m_shader->bind();

		m_shader->uploadUniformFloat3(m_backgroundTrianglesColor, "u_color");

		glm::vec3 greenColor(0.3f, 0.6f, 0.3f);
		for (int i = 0; i < 20; ++i)
		{
			for (int j = 0; j < 20; ++j)
			{
				glm::vec3 tmpScale = m_modelScale;
				tmpScale /= 10.0f;
				
				glm::vec3 tmpPos = m_modelPosition;
				tmpPos.x = tmpPos.x + (0.11f * i);
				tmpPos.y = tmpPos.y + (0.11f * j);

				glm::mat4 tmpTransform = glm::translate(glm::mat4(1.0f), tmpPos);
				tmpTransform = glm::scale(tmpTransform, tmpScale);

				renderer.submit(m_vertexArray, m_shader, tmpTransform);
			}
		}

		m_shader->uploadUniformFloat3({ 0.7f, 0.3f, 0.6f }, "u_color");
		//-- Submit data we want to render, if we wanna submit more VA's - we use more submission calls
		renderer.submit(m_vertexArray, m_shader, m_modelTransform);

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

			//-- (ImGui::CalcTextSize("Delta Time").y * 2) + 5;
			//-- 2 strings at the bottom for FPS and DeltaTime and 5 pixels to see it clear
			float textHeight = (ImGui::CalcTextSize("Delta Time").y * 2) + 5;
			ImVec2 tableSize = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - textHeight };

			if (ImGui::BeginTable("##scenePropsTable", 2, tableFlags, tableSize))
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Camera speed");

				ImGui::TableNextColumn();
				ImGui::DragFloat("##cameraSpeedScalar", &m_cameraMoveSpeed, 0.1f, 0.0f, 5.0f);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Rotation speed");

				ImGui::TableNextColumn();
				ImGui::DragFloat("##cameraRotationSpeedScalar", &m_cameraRotationSpeed, 1.0f, 0.0f, 40.0f);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Position");
				ImGui::TableNextColumn();
				ImGui::DragFloat3("##positionModel", glm::value_ptr(m_modelPosition), 0.01f, 0.0f);

				//ImGui::TableNextRow();
				//ImGui::TableNextColumn();
				//ImGui::TextUnformatted("Rotation");
				//ImGui::TableNextColumn();
				//ImGui::DragFloat3("##rotationModel", &m_modelRotation.x, 0.0f);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Scale");
				ImGui::TableNextColumn();
				ImGui::DragFloat3("##scaleModel", glm::value_ptr(m_modelScale), 0.01f, 0.0f);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Camera Position");
				ImGui::TableNextColumn();
				ImGui::DragFloat3("##cameraPosition", glm::value_ptr(m_cameraPos), 0.01f, 0.0f);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Triangles Color");
				ImGui::TableNextColumn();
				ImGui::ColorEdit3("##trianglesColor", glm::value_ptr(m_backgroundTrianglesColor));

				ImGui::EndTable();
			}

			std::string deltaTimeStr = fmt::format("Frame time: {:.1f} ms", m_deltaTime * 1000.0f);
			std::string fpsStr = fmt::format("FPS: {}", static_cast<int>(1.0f / m_deltaTime));

			ImGui::TextUnformatted(deltaTimeStr.c_str());
			ImGui::TextUnformatted(fpsStr.c_str());
			ImGui::End();
		}
	}

	void updateCamera()
	{
		float cameraSpeedWithDeltaTime = m_cameraMoveSpeed * m_deltaTime;
		float cameraRotationWithDeltaTime = m_cameraRotationSpeed * m_deltaTime;

		auto& inputPollSystem = jny::Application::subsystems().st<jny::InputPoll>();
		if (inputPollSystem.keyPressed(GLFW_KEY_RIGHT))
		{
			m_cameraPos.x += cameraSpeedWithDeltaTime;
		}
		else if (inputPollSystem.keyPressed(GLFW_KEY_LEFT))
		{
			m_cameraPos.x -= cameraSpeedWithDeltaTime;
		}

		if (inputPollSystem.keyPressed(GLFW_KEY_UP))
		{
			m_cameraPos.y += cameraSpeedWithDeltaTime;
		}
		else if (inputPollSystem.keyPressed(GLFW_KEY_DOWN))
		{
			m_cameraPos.y -= cameraSpeedWithDeltaTime;
		}

		if (inputPollSystem.keyPressed(GLFW_KEY_Q))
		{
			m_cameraRotation += cameraRotationWithDeltaTime;
		}
		else if (inputPollSystem.keyPressed(GLFW_KEY_E))
		{
			m_cameraRotation -= cameraRotationWithDeltaTime;
		}
		m_orthoCamera->setPosition(m_cameraPos);
		m_orthoCamera->setRotation(m_cameraRotation);
	}

private:
	jny::Ref<jny::Shader>				m_shader;
	jny::Ref<jny::VertexArray>			m_vertexArray;
	jny::Ref<jny::OrthographicCamera>	m_orthoCamera;

	glm::mat4							m_modelTransform = (1.0f);
	glm::vec3							m_modelPosition = { 0.0f, 0.0f, 0.0f };
	//glm::vec3							m_modelRotation = { 1.0f, 1.0f, 1.0f };
	glm::vec3							m_modelScale = { 1.0f, 1.0f, 1.0f };

	glm::vec3							m_backgroundTrianglesColor = { 0.21f, 0.73f, 0.93f };

	glm::vec3							m_cameraPos = { 0.0f, 0.0f, 0.0f };
	float								m_cameraRotation = 0.0f;

	float								m_cameraMoveSpeed = 1.8f;
	float								m_cameraRotationSpeed = 40.0f;
	float								m_deltaTime = 0.0f;
	bool								m_scenePropsWindowOpen = true;
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
