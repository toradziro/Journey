#include "jnypch.h"
#include "EditorLayer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace jny
{

EditorLayer::EditorLayer(Application* app) :
	Layer("EditorLayers"),
	m_orthoCameraCtrl(Application::aspectRatio()),
	m_app(app)
{
	JNY_ASSERT(app != nullptr, "Use valid app");
}

void EditorLayer::attach()
{
	const auto& vfs = Application::subsystems().st<VFS>();

	m_framebuffer = Framebuffer::create({});

	m_quad.m_textureOpt = TextureOpt::Textured;
	m_quad.m_position = { -1.0f, -1.0f, 0.5f };
	m_quad.m_size = { 1.0f, 1.0f, 0.0f };
	m_quad.m_texture = Texture2D::create(vfs.virtualToNativePath("assets/textures/bomb.png").string());

	m_backgroundQuad.m_textureOpt = TextureOpt::Textured;
	m_backgroundQuad.m_position = { 0.0f, 0.0f, -0.4f };
	m_backgroundQuad.m_size = { 10.0f, 10.0f, 0.0f };
	m_backgroundQuad.m_tilingFactor = 10.0f;
	m_backgroundQuad.m_texture = Texture2D::create(vfs.virtualToNativePath("assets/textures/checkerboard.png").string());
	m_checkerboardTexture = m_backgroundQuad.m_texture;

	m_orthoCameraCtrl.setZoomLevel(2.0f);

	SpriteComponent sampleSpriteComponent;
	sampleSpriteComponent.m_position = { -1.0f, -1.0f, 0.5f };
	sampleSpriteComponent.m_size = { 1.0f, 1.0f, 0.0f };
	sampleSpriteComponent.m_color = { 0.2f, 0.8f, 0.0f, 0.7f };
	//sampleSpriteComponent.m_texture = Texture2D::create(vfs.virtualToNativePath("assets/textures/bomb.png").string());

	m_scene = Ref<Scene>::create();
	auto sampleEntity = m_scene->createEntity();
	m_scene->registry().emplace<TransformComponent>(sampleEntity);
	m_scene->registry().emplace<SpriteComponent>(sampleEntity, std::move(sampleSpriteComponent));
}

void EditorLayer::detach() { }

void EditorLayer::update(f32 dt)
{
	PROFILE_FUNC;

	//-- Resizing viewport
	const auto& specs = m_framebuffer->specs();
	if (static_cast<u32>(m_viewportSize.x) != specs.m_width
		|| static_cast<u32>(m_viewportSize.y) != specs.m_height)
	{
		m_framebuffer->resize({ std::max(m_viewportSize.x, 1.0f), std::max(m_viewportSize.y, 1.0f) });
		m_orthoCameraCtrl.resize(m_viewportSize.x, m_viewportSize.y);
	}

	//-- Update camera
	if (m_viewportActive)
	{
		m_orthoCameraCtrl.update(dt);
	}

	auto& rc = Application::subsystems().st<RenderCommand>();
	auto& renderer2D = Application::subsystems().st<Renderer2D>();

	renderer2D.resetStatistics();
	m_framebuffer->bind();

	rc.setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	rc.clear();

	//-- Start rendering
	renderer2D.beginScene(m_orthoCameraCtrl.camera());

	//-- Updating our scene
	m_scene->update(dt);

	renderer2D.drawQuad(m_backgroundQuad);
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
		m_viewportActive = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
		m_app->imGuiLayer()->setUpMarkEventsProcessed(!m_viewportActive);

		ImVec2 regionSize = ImGui::GetContentRegionAvail();
		m_viewportSize = regionSize;

		u64 frameId = static_cast<u64>(m_framebuffer->colorAttachment());
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