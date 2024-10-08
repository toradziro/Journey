#include "jnypch.h"
#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{

constexpr f32 C_SPEED_ROTATION = 1.0f;

}

Sandbox2D::Sandbox2D() :
	Layer("Sandbox2Ds"),
	m_orthoCameraCtrl(jny::Application::aspectRatio())
{ }

void Sandbox2D::attach()
{
	m_quad.m_textureOpt = jny::TextureOpt::Textured;
	m_quad.m_rotateOpt = jny::RotateOpt::Rotated;
	m_quad.m_position = { -0.5f, 0.0f, 0.5f };
	m_quad.m_size = { 1.0f, 1.0f, 0.0f };
	m_quad.m_texture = jny::Texture2D::create("../resources/assets/textures/rpg_kenny_sprite_sheet.png");
	jny::subTexture(m_quad, m_sampledTexture, { 128.0f, 128.0f }, { 1.0f, 1.0f });


	m_quad1.m_textureOpt = jny::TextureOpt::Textured;
	m_quad1.m_rotateOpt = jny::RotateOpt::Rotated;
	m_quad1.m_position = { 0.8f, 0.5f, 0.5f };
	m_quad1.m_size = { 1.0f, 2.0f, 0.0f };
	m_quad1.m_texture = m_quad.m_texture;
	jny::subTexture(m_quad1, { 1.0f, 1.0f }, { 128.0f, 128.0f }, { 1.0f, 2.0f });

	m_quad2.m_textureOpt = jny::TextureOpt::Textured;
	m_quad2.m_position = { -1.0f, -1.0f, 0.5f };
	m_quad2.m_size = { 1.0f, 1.0f, 0.0f };
	m_quad2.m_texture = jny::Texture2D::create("../resources/assets/textures/bomb.png");

	m_backgroundQuad.m_textureOpt = jny::TextureOpt::Textured;
	m_backgroundQuad.m_position = { 0.0f, 0.0f, -0.4f };
	m_backgroundQuad.m_size = { 10.0f, 10.0f, 0.0f };
	m_backgroundQuad.m_tilingFactor = 10.0f;
	m_backgroundQuad.m_texture = jny::Texture2D::create("../resources/assets/textures/checkerboard.png");

	m_orthoCameraCtrl.setZoomLevel(2.0f);
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

	{
		auto& iPoll = jny::Application::subsystems().st<jny::InputPoll>();
		if (iPoll.mouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			auto [mouseX, mouseY] = iPoll.mousePos();

			auto winSizeW = jny::Application::subsystems().st<jny::Window>().width();
			auto winSizeH = jny::Application::subsystems().st<jny::Window>().height();

			const auto& cameraBounds = m_orthoCameraCtrl.bounds();
			const auto& cameraPos = m_orthoCameraCtrl.cameraPosition();

			float x = ((mouseX / winSizeW) * cameraBounds.width() - cameraBounds.width() * 0.5f) + cameraPos.x;
			float y = ((mouseY / winSizeH) * cameraBounds.height() - cameraBounds.height() * 0.5f) + cameraPos.y;

			jny::ParticleProps prop = {};
			prop.m_position = { x, y, 0.3f };
			prop.m_velocity = { 0.0f, -0.5f, 0.0f };
			prop.m_velocityVariation = { 3.0f, 1.0f };
			prop.m_colorBegin = { 0.8f, 0.2f, 0.0f, 1.0f };
			prop.m_colorEnd = { 0.2f, 0.8f, 0.0f, 1.0f };
			prop.m_sizeBegin = 0.2f;
			prop.m_sizeEnd = 0.05f;
			prop.m_sizeVariation = 0.1f;
			prop.m_lifeTime = 3.0f;

			for (int i = 0; i < 5; ++i)
			{
				m_particleSystem.emit(prop);
			}
		}
	}

	renderer2D.drawQuad(m_backgroundQuad);

	m_particleSystem.render();

	renderer2D.drawQuad(m_quad2);
	m_quad.m_rotation += C_SPEED_ROTATION * dt;
	renderer2D.drawQuad(m_quad);
	renderer2D.drawQuad(m_quad1);

	//-- End rendering
	renderer2D.endScene();
}

void Sandbox2D::onEvent(jny::Event& event)
{
	PROFILE_FUNC;

	m_orthoCameraCtrl.onEvent(event);
}

void Sandbox2D::imGuiRender()
{
	PROFILE_FUNC;

	ImGui::Begin("Color prop");
	if (ImGui::DragFloat2("Sampler", glm::value_ptr(m_sampledTexture), 1.0f, 0.0f, 20.0f))
	{
		jny::subTexture(m_quad, m_sampledTexture, { 128.0f, 128.0f }, { 1.0f, 1.0f });
	}

	ImGui::ColorEdit4("Square color", glm::value_ptr(m_quad.m_color));
	ImGui::DragFloat2("Position", glm::value_ptr(m_quad.m_position), 0.01f);
	ImGui::DragFloat2("Size", glm::value_ptr(m_quad.m_size), 0.01f);
	ImGui::DragFloat("Rotation", &m_quad.m_rotationDegrees, 1.0f);
	m_quad.m_rotation = glm::radians(m_quad.m_rotationDegrees);

	const auto& stat = jny::Application::subsystems().st<jny::Renderer2D>().stats();
	ImGui::Text("Draw calls: %d", stat.m_drawCalls);
	ImGui::Text("Quads count: %d", stat.m_quadCount);

	ImGui::End();
}
