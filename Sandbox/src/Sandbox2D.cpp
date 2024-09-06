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
	jny::Application::subsystems().st<jny::Renderer2D>().init();
}

void Sandbox2D::detach()
{
}

void Sandbox2D::update(float dt)
{
	m_orthoCameraCtrl.update(dt);

	auto& renderer2D = jny::Application::subsystems().st<jny::Renderer2D>();

	renderer2D.setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	renderer2D.clear();

	//-- Start rendering
	renderer2D.beginScene(m_orthoCameraCtrl.camera());
	renderer2D.drawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });

	//-- End rendering
	renderer2D.endScene();
	//m_shader->bind();
	//m_shader->uploadUniformFloat4(m_squareColor, "u_color");
}

void Sandbox2D::onEvent(jny::Event& event)
{
	m_orthoCameraCtrl.onEvent(event);
}

void Sandbox2D::imGuiRender()
{
	ImGui::Begin("Color prop");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_squareColor));
	ImGui::End();
}
