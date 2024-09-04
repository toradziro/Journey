#include "common.h"

#include "SandboxApplication.h"
#include "Sandbox2D.h"
#include "Journey/Core/InputPoll.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

namespace
{

constexpr const char* texturesNames[] =
{
	"resources/assets/textures/checkerboard.png",
	"resources/assets/textures/bomb.png",
	"resources/assets/textures/icon_path.png"
};

}

class ExampleLayer : public jny::Layer
{
public:

	ExampleLayer() : Layer("Test"),
		m_orthoCameraCtrl(jny::Application::aspectRatio())
	{
		//-- Vertex array
		m_vertexArray = jny::Ref<jny::VertexArray>(jny::VertexArray::create());

		//-- Vertices
		float vertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};

		//-- Vertex buffer
		jny::Ref<jny::VertexBuffer> vertexBuffer = jny::Ref<jny::VertexBuffer>(jny::VertexBuffer::create(vertices, 5 * 4));
		//-- Setting up vertex attribute array (layout for providing data splitting in shader)
		jny::BufferLayout::LayoutData layoutData = {
			{ jny::ShaderDataType::Float3, "a_Position" },
			{ jny::ShaderDataType::Float2, "a_TexturePos" },
		};
		jny::BufferLayout layout = jny::BufferLayout(std::move(layoutData));
		vertexBuffer->setLayout(layout);

		m_vertexArray->addVertexBuffer(vertexBuffer);

		//-- Indices
		uint32_t indecies[6] = { 0, 1, 2, 2, 3, 0 };
		//-- Index buffer
		jny::Ref<jny::IndexBuffer> indexBuffer = jny::Ref<jny::IndexBuffer>(jny::IndexBuffer::create(indecies, 6));

		m_vertexArray->setIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_vpMatrix;
			uniform mat4 u_modelTransform;

			out vec3 v_Position;

			void main()
			{
				gl_Position = u_vpMatrix * u_modelTransform * vec4(a_Position, 1.0f);
				v_Position = a_Position;
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			uniform vec3 u_color;

			void main()
			{
				color = vec4(u_color, 1.0f);
			}
		)";

		auto& shaderLib = jny::Application::subsystems().st<jny::ShaderLibrary>();

		m_shader = jny::Ref<jny::Shader>(jny::Shader::create("Triangle shader", std::move(vertexSrc), std::move(fragmentSrc)));
		shaderLib.add(m_shader);

		m_textureShader = shaderLib.load("resources/assets/shaders/Texture.glsl");
		m_bombTexture = jny::Texture2D::create("resources/assets/textures/bomb.png");
		m_frontObjTexture = jny::Texture2D::create(texturesNames[m_currSelectedTexture]);

		m_textureShader->bind();
		m_textureShader->uploadUniformInt(0, "u_texture");
	}

	void update(float dt) override
	{
		m_orthoCameraCtrl.update(dt);

		m_deltaTime = dt;

		auto& renderer = jny::Application::subsystems().st<jny::Renderer>();

		renderer.setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		renderer.clear();

		//-- Start rendering
		renderer.beginScene(m_orthoCameraCtrl.camera());

		m_modelTransform = glm::mat4(1.0f);
		m_modelTransform = glm::translate(m_modelTransform, m_modelPosition);
		m_modelTransform = glm::scale(m_modelTransform, m_modelScale);
		
		m_bombTexture->bind();
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

				renderer.submit(m_vertexArray, /*m_shader*/m_textureShader, tmpTransform);
			}
		}

		m_frontObjTexture->bind();
		//-- Submit data we want to render, if we wanna submit more VA's - we use more submission calls
		renderer.submit(m_vertexArray, m_textureShader/*m_shader*/, m_modelTransform);

		//-- End rendering
		renderer.endScene();
	}

	void onEvent(jny::Event& event) override
	{
		m_orthoCameraCtrl.onEvent(event);
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
				ImGui::TextUnformatted("Position");
				ImGui::TableNextColumn();
				ImGui::DragFloat3("##positionModel", glm::value_ptr(m_modelPosition), 0.01f, 0.0f);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Scale");
				ImGui::TableNextColumn();
				ImGui::DragFloat3("##scaleModel", glm::value_ptr(m_modelScale), 0.01f, 0.0f);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Front Texture");
				ImGui::TableNextColumn();

				if (ImGui::Combo("##textureSelector", &m_currSelectedTexture, texturesNames, IM_ARRAYSIZE(texturesNames)))
				{
					m_frontObjTexture = jny::Texture2D::create(texturesNames[m_currSelectedTexture]);
				}

				ImGui::EndTable();
			}

			std::string deltaTimeStr = fmt::format("Frame time: {:.1f} ms", m_deltaTime * 1000.0f);
			std::string fpsStr = fmt::format("FPS: {}", static_cast<int>(1.0f / m_deltaTime));

			ImGui::TextUnformatted(deltaTimeStr.c_str());
			ImGui::TextUnformatted(fpsStr.c_str());
			ImGui::End();
		}
	}

private:
	jny::OrthographicCameraController	m_orthoCameraCtrl;

	jny::Ref<jny::Shader>				m_shader;
	jny::Ref<jny::Shader>				m_textureShader;
	jny::Ref<jny::Texture2D>			m_bombTexture;
	jny::Ref<jny::Texture2D>			m_frontObjTexture;
	jny::Ref<jny::VertexArray>			m_vertexArray;

	glm::mat4							m_modelTransform = (1.0f);
	glm::vec3							m_modelPosition = { 0.0f, 0.0f, 0.0f };
	glm::vec3							m_modelScale = { 1.0f, 1.0f, 1.0f };

	float								m_deltaTime = 0.0f;

	int									m_currSelectedTexture = 0;

	bool								m_scenePropsWindowOpen = true;
};

Sandbox::Sandbox()
{
	//this->pushLayer<ExampleLayer>();
	this->pushLayer<Sandbox2D>();
}

Sandbox::~Sandbox()
{

}

jny::Application* jny::createApplication()
{
	return new Sandbox();
}
