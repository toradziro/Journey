#include "jnypch.h"
#include "EditorLayer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace jny
{

EditorLayer::EditorLayer() :
	Layer("EditorLayers"),
	m_orthoCameraCtrl(Application::aspectRatio())
{ }

void EditorLayer::attach()
{
	FramebufferSpecs specs;
	specs.m_width = 1200;
	specs.m_height = 800;
	m_framebuffer = Framebuffer::create(specs);

	m_quad.m_textureOpt = TextureOpt::Textured;
	m_quad.m_position = { -1.0f, -1.0f, 0.5f };
	m_quad.m_size = { 1.0f, 1.0f, 0.0f };
	m_quad.m_texture = Texture2D::create("../resources/assets/textures/bomb.png");

	m_backgroundQuad.m_textureOpt = TextureOpt::Textured;
	m_backgroundQuad.m_position = { 0.0f, 0.0f, -0.4f };
	m_backgroundQuad.m_size = { 10.0f, 10.0f, 0.0f };
	m_backgroundQuad.m_tilingFactor = 10.0f;
	m_backgroundQuad.m_texture = Texture2D::create("../resources/assets/textures/checkerboard.png");
	m_checkerboardTexture = m_backgroundQuad.m_texture;

	m_orthoCameraCtrl.setZoomLevel(2.0f);
}

void EditorLayer::detach() { }

void EditorLayer::update(float dt)
{
	PROFILE_FUNC;

	m_orthoCameraCtrl.update(dt);
	m_particleSystem.update(dt);

	auto& rc = Application::subsystems().st<RenderCommand>();
	auto& renderer2D = Application::subsystems().st<Renderer2D>();

	renderer2D.resetStatistics();
	m_framebuffer->bind();

	rc.setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	rc.clear();

	//-- Start rendering
	renderer2D.beginScene(m_orthoCameraCtrl.camera());

	{
		auto& iPoll = Application::subsystems().st<InputPoll>();
		if (iPoll.mouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			auto [mouseX, mouseY] = iPoll.mousePos();

			auto winSizeW = Application::subsystems().st<Window>().width();
			auto winSizeH = Application::subsystems().st<Window>().height();

			const auto& cameraBounds = m_orthoCameraCtrl.bounds();
			const auto& cameraPos = m_orthoCameraCtrl.cameraPosition();

			float x = ((mouseX / winSizeW) * cameraBounds.width() - cameraBounds.width() * 0.5f) + cameraPos.x;
			float y = ((mouseY / winSizeH) * cameraBounds.height() - cameraBounds.height() * 0.5f) + cameraPos.y;

			ParticleProps prop = {};
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
	renderer2D.drawQuad(m_quad);

	//-- End rendering
	renderer2D.endScene();
	m_framebuffer->unbind();

	m_FPS = 1.0f / dt;
}

void EditorLayer::onEvent(Event& event)
{
	PROFILE_FUNC;

	m_orthoCameraCtrl.onEvent(event);
}

void EditorLayer::imGuiRender()
{
	PROFILE_FUNC;

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("TstOpt", NULL, &m_tstOpt);
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	ImGui::Begin("TstWindow");

	if (ImGui::Begin("Viewport"))
	{
		ImVec2 regionSize = ImGui::GetContentRegionAvail();
		if (regionSize != m_viewportSize)
		{
			m_viewportSize = regionSize;
			m_framebuffer->resize({ m_viewportSize.x, m_viewportSize.y });
			m_orthoCameraCtrl.resize(m_viewportSize.x, m_viewportSize.y);
		}
		uint64_t frameId = static_cast<uint64_t>(m_framebuffer->colorAttachment());
		ImGui::Image(reinterpret_cast<void*>(frameId), m_viewportSize, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });
		ImGui::End();
	}

	ImGui::DragFloat2("Position", glm::value_ptr(m_quad.m_position), 0.01f);
	ImGui::Text("FPS: %d", static_cast<int>(m_FPS));

	const auto& stat = Application::subsystems().st<Renderer2D>().stats();
	ImGui::Text("Draw calls: %d", stat.m_drawCalls);
	ImGui::Text("Quads count: %d", stat.m_quadCount);

	ImGui::End();
}

}