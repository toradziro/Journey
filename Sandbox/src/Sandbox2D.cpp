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
	m_quad.m_rotation = 0.0f;
	m_quad.m_tilingFactor = 1.0f;

	m_backgroundQuad.m_textureOpt = jny::TextureOpt::Textured;
	m_backgroundQuad.m_rotateOpt = jny::RotateOpt::AlignedByAxices;
	m_backgroundQuad.m_position = { 0.0f, 0.0f, -0.1f };
	m_backgroundQuad.m_size = { 10.0f, 10.0f, 0.0f };
	m_backgroundQuad.m_tilingFactor = 10.0f;
	m_backgroundQuad.m_texture = jny::Texture2D::create("resources/assets/textures/checkerboard.png");
	m_backgroundQuad.m_color = { 0.1f, 0.3f, 0.1f, 1.0f };
}

void Sandbox2D::detach() { }

void Sandbox2D::update(float dt)
{
	PROFILE_FUNC;

	m_orthoCameraCtrl.update(dt);

	auto& rc = jny::Application::subsystems().st<jny::RenderCommand>();
	rc.setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	rc.clear();

	auto& renderer2D = jny::Application::subsystems().st<jny::Renderer2D>();

	//-- Start rendering
	renderer2D.beginScene(m_orthoCameraCtrl.camera());

	renderer2D.drawQuad(m_backgroundQuad);
	renderer2D.drawQuad(m_quad);

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
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_quad.m_color));
	ImGui::DragFloat2("Position", glm::value_ptr(m_quad.m_position), 0.01f);
	ImGui::DragFloat2("Size", glm::value_ptr(m_quad.m_size), 0.01f);
	ImGui::DragFloat("Rotation", &m_quad.m_rotationDegrees, 1.0f);
	m_quad.m_rotation = glm::radians(m_quad.m_rotationDegrees);

	ImGui::End();
}
