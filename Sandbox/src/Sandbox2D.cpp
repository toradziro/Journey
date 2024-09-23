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
	m_quad.m_textureOpt = jny::TextureOpt::FlatColored;
	m_quad.m_rotateOpt = jny::RotateOpt::Rotated;
	m_quad.m_color = { 0.2f, 0.3f, 0.8f, 0.7f };
	m_quad.m_position = { 0.0f, 0.0f, 0.0f };
	m_quad.m_size = { 1.0f, 1.0f, 0.0f };

	m_quad2.m_textureOpt = jny::TextureOpt::Textured;
	m_quad2.m_position = { -1.0f, -1.0f, -0.2f };
	m_quad2.m_size = { 1.0f, 1.0f, 0.0f };
	m_quad2.m_texture = jny::Texture2D::create("resources/assets/textures/bomb.png");

	m_backgroundQuad.m_textureOpt = jny::TextureOpt::Textured;
	m_backgroundQuad.m_position = { 0.0f, 0.0f, -0.4f };
	m_backgroundQuad.m_size = { 10.0f, 10.0f, 0.0f };
	m_backgroundQuad.m_tilingFactor = 10.0f;
	m_backgroundQuad.m_texture = jny::Texture2D::create("resources/assets/textures/checkerboard.png");
}

void Sandbox2D::detach() { }

void Sandbox2D::update(float dt)
{
	PROFILE_FUNC;

	m_orthoCameraCtrl.update(dt);
	m_particleSystem.update(dt);

	auto& rc = jny::Application::subsystems().st<jny::RenderCommand>();
	rc.setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	rc.clear();

	auto& renderer2D = jny::Application::subsystems().st<jny::Renderer2D>();

	renderer2D.resetStatistics();
	//-- Start rendering
	renderer2D.beginScene(m_orthoCameraCtrl.camera());

	renderer2D.drawQuad(m_backgroundQuad);
	renderer2D.drawQuad(m_quad2);
	renderer2D.drawQuad(m_quad);

	m_particleSystem.render();
	//-- End rendering
	renderer2D.endScene();

	m_FPS = 1.0f / dt;
}

void Sandbox2D::onEvent(jny::Event& event)
{
	PROFILE_FUNC;

	m_orthoCameraCtrl.onEvent(event);
	jny::EventDispatcher dispatcher(event);
	dispatcher.dispatch<jny::MouseButtonPressedEvent>([this](jny::MouseButtonPressedEvent& e)
		{
			auto& iPoll = jny::Application::subsystems().st<jny::InputPoll>();
			
			auto winSizeW = jny::Application::subsystems().st<jny::Window>().width();
			auto winSizeH = jny::Application::subsystems().st<jny::Window>().height();

			float ar = jny::Application::aspectRatio();

			jny::ParticleProps prop = {};
			prop.m_position = { m_quad.m_position.x, m_quad.m_position.y, 0.2f };
			prop.m_velocity = { 0.0f, -0.5f, 0.0f };
			prop.m_velocityVariation = { 6.0f, 6.0f };
			prop.m_colorBegin = { 0.8f, 0.2f, 0.0f, 1.0f };
			prop.m_colorEnd = { 0.2f, 0.8f, 0.0f, 1.0f };
			prop.m_sizeBegin = 0.05f;
			prop.m_sizeEnd = 0.05f;
			prop.m_sizeVariation = 0.1f;
			prop.m_lifeTime = 10.0f;
			
			for (int i = 0; i < 1000; ++i)
			{
				m_particleSystem.emit(prop);
			}
			return false;
		});
}

void Sandbox2D::imGuiRender()
{
	PROFILE_FUNC;

	ImGui::Begin("Color prop");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_quad.m_color));
	ImGui::DragFloat2("Position", glm::value_ptr(m_quad.m_position), 0.01f);
	ImGui::DragFloat2("Size", glm::value_ptr(m_quad.m_size), 0.01f);
	ImGui::DragFloat("Rotation", &m_quad.m_rotationDegrees, 1.0f);
	m_quad.m_rotation = glm::radians(m_quad.m_rotationDegrees);

	ImGui::Text("FPS: %d", static_cast<int>(m_FPS));

	const auto& stat = jny::Application::subsystems().st<jny::Renderer2D>().stats();
	ImGui::Text("Draw calls: %d", stat.m_drawCalls);
	ImGui::Text("Quads count: %d", stat.m_quadCount);

	ImGui::End();
}
