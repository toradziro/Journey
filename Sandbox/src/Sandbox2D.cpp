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
	m_checkerboardTexture = jny::Texture2D::create("resources/assets/textures/checkerboard.png");
}

void Sandbox2D::detach() { }

void Sandbox2D::update(float dt)
{
	m_orthoCameraCtrl.update(dt);

	auto& rc = jny::Application::subsystems().st<jny::RenderCommand>();
	rc.setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	rc.clear();

	auto& renderer2D = jny::Application::subsystems().st<jny::Renderer2D>();

	//-- Start rendering
	renderer2D.beginScene(m_orthoCameraCtrl.camera());
	renderer2D.drawQuad(m_pos, m_size, m_squareColor);
	renderer2D.drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_checkerboardTexture);

	//-- End rendering
	renderer2D.endScene();
}

void Sandbox2D::onEvent(jny::Event& event)
{
	m_orthoCameraCtrl.onEvent(event);
}

void Sandbox2D::imGuiRender()
{
	ImGui::Begin("Color prop");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_squareColor));
	ImGui::DragFloat2("Position: ", glm::value_ptr(m_pos), 0.01f);
	ImGui::DragFloat2("Size: ", glm::value_ptr(m_size), 0.01f);
	ImGui::End();
}
